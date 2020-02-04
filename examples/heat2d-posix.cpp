/**
 *  @file   heat2d.cpp
 *  @author Leonardo A. Bautista Gomez and Sheng Di and Kai Keller
 *  @date   July, 2019
 *  @brief  Heat distribution code in C++ to test FTI.
 */

#include <hdf5.h>
#include <fti.h>
#include <interface-paper.h>
#include <mpi.h>
//#include <mpi-ext.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <random>
#include "../deps/iniparser/iniparser.h"
#include "../deps/iniparser/dictionary.h"
#include <chrono>
#include <sstream>
#include <fstream>

typedef enum CLOCK_ID {
    ITER,
    CKPT,
    RECO,
    EXEC,
    FTI_INIT,
    NUM_CLOCK_IDS
} CLOCK_ID;

class Timer{
  public:
    Timer() {
        _t0 = std::chrono::system_clock::now();
        _dt = new double[NUM_CLOCK_IDS];
        _t  = new double[NUM_CLOCK_IDS];
        std::fill( _t, _t+NUM_CLOCK_IDS, 0 );
        _tp = new std::chrono::time_point<std::chrono::system_clock>[NUM_CLOCK_IDS];
    }
    
    ~Timer() {
        delete[] _dt;
        delete[] _t;
        delete[] _tp;
    };

    inline void start(const int i) { 
        _tp[i] = std::chrono::system_clock::now(); 
    }
    
    inline void stop (const int i) {
      std::chrono::duration<double> tmp = std::chrono::system_clock::now() - _tp[i];
      _dt[i] = tmp.count();
      _t [i]+= _dt[i];
    }
    
    inline double get_t(const int i) {
        return _t[i]; 
    }
    
    inline double get_dt(const int i) {
        return _dt[i]; 
    }

    inline double get_dT() { 
        return  std::chrono::duration<double>(std::chrono::system_clock::now() - _t0).count(); 
    }

    inline double log(const int i, std::string msg) { 
        double dt, dT, t;
        auto now = std::chrono::system_clock::now();
        dt = get_dt( i ); 
        dT = get_dT();
        t = get_t( i );
        _log << msg << " (total_time | delta_t | system_time_stamp) : "; 
        _log << t;  
        _log << " : " << dt; 
        _log << " : " << std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count(); 
        _log << std::endl;
    }

    inline void flush( std::string fn, const bool write ) {
        if( write ) {
            std::ofstream instrumentFile( fn, std::ofstream::app );
            instrumentFile << _log.str();
            instrumentFile.close();
            _log.str(std::string());
        }
    }

  private:
    std::chrono::time_point<std::chrono::system_clock> _t0;
    std::chrono::time_point<std::chrono::system_clock> *_tp;
    double *_dt;
    double *_t;
    std::stringstream _log;
};

#define DEBUG(A) std::cout << A << std::endl;

// GRID PARAMETERS
//const size_t M = 1024;
//const size_t N = 1024;
size_t M = (768*128);               // largest (768*256);
size_t N = (768*128);               // largest (768*256);

// SIMULATION PARAMETERS
std::string m_logdir;
int ITER_MAX;
const int ITER_OUT = 1;
const double PRECISION = 0.000;

// FAULT TOLERANCE
int ITER_CHK;
int ITER_FAIL;

class SEnvironment {

    public:
        
        void init( int & argc, char** & argv ) {
            
            if( argc != 7 ) {
                printf("usage: %s crash IterMax IterChk IterFail", argv[0]);
                exit(-1);
            }
            
            m_crash   = atoi( argv[1] );
            ITER_MAX  = atoi( argv[2] );
            ITER_CHK  = atoi( argv[3] );
            ITER_FAIL = atoi( argv[4] );
            M *= atoi( argv[5] );
            std::stringstream ss;
            ss << argv[6] << "/timing/";
            m_logdir = ss.str();
            ss.str(std::string());
            
            Timer timestamp;
            timestamp.start(EXEC); 
            timestamp.stop(EXEC);
            timestamp.log(EXEC, "T1" );
            
            MPI::Init( argc, argv );
            m_global_comm = MPI_COMM_WORLD;
            //MPI_Comm_set_errhandler( MPI_COMM_WORLD, MPI::ERRORS_THROW_EXCEPTIONS );
           
            MPI_Comm_group( MPI_COMM_WORLD, &m_world_group );
            
            MPI_Comm_size(MPI_COMM_WORLD, &m_global_size);
            MPI_Comm_rank(MPI_COMM_WORLD, &m_global_rank);

            ss << argv[6] << "/fti.inp";
            dictionary* ini = iniparser_load( ss.str().c_str() );
            m_nodeSize = iniparser_getint( ini, "Basic:node_size", -1 );
            Timer ftiinit;
            ftiinit.start(FTI_INIT);
            int rval = FTI_Init( ss.str().c_str(), MPI_COMM_WORLD );
            ftiinit.stop(FTI_INIT);
            ss.str(std::string());
            
            if(rval!=FTI_SCES) exit(0);
            m_restart = ( FTI_Status() > 0 );
            
            m_comm = FTI_COMM_WORLD;
            MPI_Comm_rank( m_comm, &m_rank );
            MPI_Comm_size( m_comm, &m_size );
            
            ss << m_logdir << "timestamps";
            timestamp.flush( ss.str(), m_rank == 0 );
            ss.str(std::string());
           
            ss << m_logdir << "rank-" << m_rank;
            ftiinit.log( FTI_INIT, "FTI_Init" );
            ftiinit.flush( ss.str(), 1 );
            ss.str(std::string());
            
        }
        
        void finalize( void ) {
            
            FTI_Finalize();
            
            Timer loc;
            loc.start(EXEC); 
            loc.stop(EXEC);
            loc.log(EXEC, "T2" );
            std::stringstream ss;
            ss << m_logdir << "timestamps";
            loc.flush( ss.str(), m_rank == 0 );
            ss.str(std::string());
            
            MPI_Finalize();
        
        }

        bool restart() { 
            bool restart = m_restart;
            m_restart = false;
            return restart; 
        }
        
        const bool & head() const { return m_head; }
        const bool & crash() const { return m_crash; }
        
        const int & rank() const { return m_rank; }
        
        const int & size() const { return m_size; }
        
        const MPI_Comm & comm() const { return m_comm; }
        
        void reset( void ) {
            
            MPI_Comm new_global_comm;
            //MPIX_Comm_shrink( m_global_comm, &new_global_comm );
            //MPI_Barrier( new_global_comm );
            //MPI_Comm_free( &m_global_comm );
            //MPI_Comm_rank( m_global_comm, &m_global_rank );
          
            MPI_Group new_world_group;
            int* ranks = (int*)malloc( sizeof(int)*(m_global_size - m_nodeSize) );
            int i;
            for(i=m_nodeSize;i<m_global_size;i++) ranks[i-m_nodeSize] = i;
            MPI_Group_incl( m_world_group, m_global_size-m_nodeSize, ranks, &new_world_group );

            MPI_Comm_create_group( m_global_comm, new_world_group, 0, &new_global_comm );
             
            m_global_comm = new_global_comm;
            
            MPI_Barrier( m_global_comm );
            
            MPI_Comm_rank( m_global_comm, &m_global_rank );
            MPI_Comm_size( m_global_comm, &m_global_size );
            
            // enable VPR restart
            if( m_global_rank == 0 ) XFTI_updateKeyCfg( "Restart", "failure", "3" ); 
            if( m_global_rank == 0 ) std::cout << "==========================================================================" << std::endl;
            if( m_global_rank == 0 ) std::cout << ":: Error detected, try to recover... " << std::endl;

            int size; MPI_Comm_size( m_global_comm, &size );
            MPI_Barrier( m_global_comm );
            // reset FTI
            //MPI_Comm_free(&FTI_COMM_WORLD);
            
            if( m_global_rank == 0 ) {
                std::cout << "   :::   WORLD SIZE = " << size << "   :::   " << std::endl;
            }

            FTI_Init( "fti.inp", m_global_comm );
            m_comm = FTI_COMM_WORLD;
            MPI_Comm_rank( m_comm, &m_rank );
            MPI_Comm_size( m_comm, &m_size );
            
            m_crash = false;
            
            MPI_Barrier( m_comm );

        }

    private:

        bool m_restart;
        bool m_head;
        MPI_Comm m_comm;
        MPI_Comm m_global_comm;
        int m_rank;
        int m_global_rank;
        int m_size;
        int m_global_size;
        double m_t0;
        bool m_crash;
        int m_nodeSize;
        MPI_Group m_world_group;

};

class TDist {
    
    public:
        
        void print_progress( int i, const SEnvironment & env ) {
            
            if( env.head() ) throw MPI::Exception(0);
            if( (i%ITER_OUT == 0) && (env.rank() == 0) )
                std::cout << "Step : " << i << ", current error = " << m_error << "; target = " << PRECISION << std::endl;
        
        }
        
        void init( const size_t & M, const size_t & N, const SEnvironment & env ) {
            
            if( env.head() ) return;
            m_chk_id = 1;
            m_Mloc = ( M / env.size() );
            int Mloc_rest = M%static_cast<size_t>(env.size());
            if( env.rank() < Mloc_rest ) m_Mloc += 1;
            m_Nloc = N;

            //int sum = 0;
            //MPI_Allreduce( &m_Mloc, &sum, 1, MPI_INT, MPI_SUM, env.comm() );
            //if( env.rank() == 0 ) DEBUG( "M: " << M << " Mloc: " << m_Mloc << " sum: " << sum );
            
            m_num_ghosts = 0;
            m_max_dist_row = m_Mloc-1; 
            m_max_data_row = m_Mloc-1;
            
            m_has_down_neighbor = ( env.rank() > 0 ) ? true : false ; 
            m_has_up_neighbor = ( env.rank() < (env.size()-1) ) ? true : false ;
            if( m_has_down_neighbor ) {
                m_num_ghosts++;
                m_max_dist_row++;
            }
            if( m_has_up_neighbor ) {
                m_num_ghosts++;
                m_max_dist_row++;
            }
            //std::cout << "   :::   [" << env.rank() << "]   m_Mloc = " << m_Mloc << "   :::" << std::endl;
            //std::cout << "   :::   [" << env.rank() << "]   down neighbor = " << m_has_down_neighbor << "   :::" << std::endl;
            //std::cout << "   :::   [" << env.rank() << "]   up neighbor = " << m_has_up_neighbor << "   :::" << std::endl;
            //std::cout << "   :::   [" << env.rank() << "]   ghosts = " << m_num_ghosts << "   :::" << std::endl;
            //std::cout << "   :::   [" << env.rank() << "]   max row = " << m_max_dist_row << "   :::" << std::endl;
            alloc();
            init_data( env );
            
            hsize_t dim_i = 1;
            hsize_t dim_data[2] = { M, N };
            FTI_DefineGlobalDataset( 0, 1, &dim_i, "iteration counter", NULL, FTI_INTG );
            FTI_DefineGlobalDataset( 1, 2, dim_data, "temperature distribution (h)", NULL, FTI_DBLE );

        }
        
        void alloc( void ) {
            
            m_dist = new double*[ m_Mloc + m_num_ghosts ];
            m_dist_cpy = new double*[ m_Mloc + m_num_ghosts ];
            m_data = new double[ m_Mloc * m_Nloc ];
            if( m_has_down_neighbor ) {
                m_ghost_down = new double[ m_Nloc ];
                m_dist[0] = m_ghost_down;
                m_dist_cpy[0] = new double[ m_Nloc ];
            }
            if( m_has_up_neighbor ) { 
                m_ghost_up = new double[ m_Nloc ];
                m_dist[m_max_dist_row] = m_ghost_up;
                m_dist_cpy[m_max_dist_row] = new double[ m_Nloc ];
            }
            for( int m = (m_has_down_neighbor ? 1 : 0), _m = 0; _m <= m_max_data_row; ++m, ++_m ) {
                m_dist[m] = &m_data[(_m)*m_Nloc];
                m_dist_cpy[m] = new double[ m_Nloc ];
            }
        
        }
        
        void init_data( const SEnvironment & env ) { 
            
            std::uniform_real_distribution<double> random(0,1000);
            std::default_random_engine re(31071980);

            for (size_t m = 0; m <= m_max_dist_row; ++m) {
                for (size_t n = 0; n < m_Nloc; n++) {
                    m_dist[m][n] = 0;
                }
            }

            size_t range_begin = env.rank() * (M/env.size());
            size_t range_end = range_begin + m_Mloc;
            size_t minM = M*0.2;
            size_t maxM = M*0.8;
            
            if ( (range_end >= minM) && (range_begin <= maxM) ) {
                size_t minN = m_Nloc*0.2;
                size_t maxN = m_Nloc*0.8;
                for(size_t m = 0, pos = range_begin; m <= m_max_dist_row; ++m, ++pos) { 
                    for (size_t n = minN; n < maxN; ++n) {
                        if( pos >= minM && pos <= maxM ) {
                            m_dist[m][n] = pos%static_cast<size_t>(M);
                        }   
                    }
                }
            }

        }

        void compute_step( const SEnvironment & env ) {
            
            if( env.head() ) return;
            MPI_Request req1[2], req2[2];
            MPI_Status status1[2], status2[2];
            double localerror;
            
            localerror = 0;
            
            for(int m = 0; m <= m_max_dist_row; ++m) {
                for(int n = 0; n < m_Nloc; ++n) {
                    m_dist_cpy[m][n] = m_dist[m][n];
                }
            }
            
            if ( m_has_down_neighbor ) {
                MPI_Isend(&m_dist[1][0], m_Nloc, MPI_DOUBLE, env.rank()-1, 0, env.comm(), &req1[0]);
                MPI_Irecv(&m_dist_cpy[0][0],   m_Nloc, MPI_DOUBLE, env.rank()-1, 0, env.comm(), &req1[1]);
            }
            
            if ( m_has_up_neighbor ) {
                MPI_Isend(&m_dist[m_max_dist_row-1][0], m_Nloc, MPI_DOUBLE, env.rank()+1, 0, env.comm(), &req2[0]);
                MPI_Irecv(&m_dist_cpy[m_max_dist_row][0], m_Nloc, MPI_DOUBLE, env.rank()+1, 0, env.comm(), &req2[1]);
            }
            
            if ( m_has_down_neighbor ) {
                MPI_Waitall(2,req1,status1);
            }
            
            if ( m_has_up_neighbor ) {
                MPI_Waitall(2,req2,status2);
            }
            
            for (int m = (m_has_down_neighbor ? 1 : 0); m <= (m_has_up_neighbor ? m_max_dist_row-1 : m_max_dist_row); ++m) {
                for (int n=0; n<m_Nloc; ++n) {
                    double val = m_dist_cpy[m][n]; 
                    int norm = 1;
                    if( m > 0 ) {
                        val += m_dist_cpy[m-1][n];
                        norm++;
                    }
                    if( m < m_max_dist_row ) {
                        val += m_dist_cpy[m+1][n];
                        norm++;
                    }
                    if( n > 0 ) {
                        val += m_dist_cpy[m][n-1];
                        norm++;
                    }
                    if( n < m_Nloc-1 ) {
                        val += m_dist_cpy[m][n+1];
                        norm++;
                    }
                    m_dist[m][n] = val/norm;
                    double error = std::fabs( m_dist[m][n] - m_dist_cpy[m][n] ); 
                    if ( error > localerror ) {
                        localerror = error; 
                    }
                }
            }

            double globalerror;
            MPI_Allreduce( &localerror, &globalerror, 1, MPI_DOUBLE, MPI_MAX, env.comm() );

            m_error = globalerror;

        }
        
        void checkpoint( int & i, SEnvironment & env, bool force=false ) {
            
            if( env.head() ) return;
            if( ((i%ITER_CHK == 0) && (i>0)) || force ) {
                m_clock.start(CKPT);
                FTI_InitICP( m_chk_id++, 4, 1 );  
                //FTI_InitICP( m_chk_id++, 4, 1 );  
                //FTI_Checkpoint( m_chk_id++, FTI_L4_H5_SINGLE );
                FTI_AddVarICP( 0 );
                FTI_AddVarICP( 1 );
                FTI_FinalizeICP();
                m_clock.stop(CKPT);
                std::stringstream ss;
                ss << "Checkpoint[" << i << "]";
                m_clock.log(CKPT, ss.str());
                ss.str(std::string());
                ss << m_logdir << "rank-" << env.rank();
                m_clock.flush( ss.str(), 1 );
                ss.str(std::string());
            }
        
        }

        void protect( int & i, SEnvironment & env ) {
            
            if( env.head() ) return;
            int id = 0;

            hsize_t offset_i = 0;
            hsize_t count_i = 1;
            FTI_Protect(id, &i, 1, FTI_INTG);
            FTI_AddSubset( id, 1, &offset_i, &count_i, 0 );
            id += 1;

            hsize_t stride = M / env.size();
            hsize_t offset_M = stride * env.rank();
            //std::cout << "   :::   [" << env.rank() << "]   stride = " << stride << "   :::" << std::endl;
            hsize_t offset_data[2] = { offset_M, 0 };
            hsize_t count_data[2] = { m_Mloc, m_Nloc };
            FTI_Protect(id, m_data, m_Mloc*m_Nloc, FTI_DBLE);
            FTI_AddSubset( id, 2, offset_data, count_data, 1 );
        
        }
        
        double get_error( void ) { return m_error; } 
        
        void finalize( void ) {
           
            delete m_data;
            if( m_has_up_neighbor ) delete m_ghost_up;
            if( m_has_down_neighbor ) delete m_ghost_down;
            delete m_dist;
            delete[] m_dist_cpy; 

        }
        
        bool condition() {
            
            return m_error < PRECISION;
        
        }
        
        void handle_error( int & i, SEnvironment & env ) {
            
            //if( !env.head() ) XFTI_LiberateHeads();
            int chk_id = m_chk_id;
            // reset environment
            env.reset();
            //sleep(2);            

            // reset pointers
            finalize();

            // initialize
            init( M, N, env );
            protect( i, env );
            FTI_Recover();
            if( env.rank() == 0 ) std::cout << ":: Recovery successful! " << std::endl;
            if( env.rank() == 0 ) std::cout << "==========================================================================" << std::endl;

            m_chk_id = chk_id;

        }

        bool recover( SEnvironment & env ) {
            if( env.restart() ) {
                m_clock.start(RECO);
                FTI_Recover();
                m_clock.stop(RECO);
                m_clock.log(RECO, std::string("Recovery"));
                std::stringstream ss;
                ss.str(std::string());
                ss << m_logdir << "rank-" << env.rank();
                m_clock.flush( ss.str(), 1 );
                ss.str(std::string());
                return true;
            }
            return false;
        }
        void inject_failure( int i, SEnvironment & env, int rank = 0 ) {
            
            int r,s;
            if( env.head() ) return;
            if( i%ITER_FAIL == 0 && i>0 && env.crash() ) {
                XFTI_CreateHostfile( 15, NULL, "my_hostfile" );
                MPI_Barrier(FTI_COMM_WORLD);
                XFTI_CrashNodes(1);
                //XFTI_LiberateHeads();
                throw MPI::Exception(0);
                //MPI_Allreduce( &r, &s, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
            }//if( (i%ITER_FAIL == 0) && (i>0) && (env.rank() == rank) && env.crash() ) {
            //    *(int*)NULL = 1;
            //}
        
        }

        Timer & clock() { return m_clock; }

    private:

        int m_chk_id;

        double m_error;
        double** m_dist;
        double** m_dist_cpy;
        double* m_data;
        double* m_ghost_down;
        double* m_ghost_up;
        int m_num_ghosts;
        
        Timer m_clock;
        size_t m_Mloc;
        size_t m_Nloc;

        bool m_has_up_neighbor;
        bool m_has_down_neighbor;

        size_t m_max_dist_row;
        size_t m_max_data_row;

};

static TDist dist;
static SEnvironment env;

int main( int argc, char** argv )
{

    int i;

    // INITIALIZE SIMULATION
    env.init( argc, argv );
    dist.init( M, N, env );
    dist.protect( i, env );
        
    // MAINLOOP
    for(i=0; i<ITER_MAX; ++i) {
        

        dist.checkpoint( i, env );
        dist.recover( env );
 
        try {

            dist.inject_failure( i, env );
            
            dist.clock().start(ITER);
            dist.compute_step( env );
            std::stringstream ss;
            ss << "Iteration[" << i << "]";
            dist.clock().stop( ITER );
            dist.clock().log( ITER, ss.str() );

            dist.print_progress( i, env );
        
        } catch ( MPI::Exception ) {
             
            dist.handle_error( i, env );
            dist.compute_step( env );
            dist.print_progress( i, env );
        
        }
        
        if( dist.condition() ) break;
        
    }
    
    // FINALIZE
    dist.finalize();
    std::stringstream ss;
    ss << m_logdir << "rank-" << env.rank();
    dist.clock().flush( ss.str(), 1 );
    ss.str(std::string());
    env.finalize();
        
    return 0;

}