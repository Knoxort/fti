/**
 *  Copyright (c) 2017 Leonardo A. Bautista-Gomez
 *  All rights reserved
 *
 *  FTI - A multi-level checkpointing library for C/C++/Fortran applications
 *
 *  Revision 1.0 : Fault Tolerance Interface (FTI)
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its contributors
 *  may be used to endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  @author Kai Keller (kellekai@gmx.de)
 *  @file   ftiff.h
 *  @date   October, 2017
 *  @brief  Header file for the FTI File Format (FTI-FF).
 */

#ifndef _FTIFF_H
#define _FTIFF_H

#ifndef FTI_NOZLIB
#   include "zlib.h"
#endif
#include <assert.h>
#include <string.h>


void FTIFF_InitMpiTypes();
int FTIFF_DeserializeFileMeta( FTIFF_metaInfo* meta, char* buffer_ser );
int FTIFF_DeserializeDbMeta( FTIFF_db* db, char* buffer_ser );
int FTIFF_DeserializeDbVarMeta( FTIFF_dbvar* dbvar, char* buffer_ser );
int FTIFF_SerializeFileMeta( FTIFF_metaInfo* meta, char* buffer_ser );
int FTIFF_SerializeDbMeta( FTIFF_db* db, char* buffer_ser );
int FTIFF_SerializeDbVarMeta( FTIFF_dbvar* dbvar, char* buffer_ser );
void FTIFF_FreeDbFTIFF(FTIFF_db* last);
int FTIFF_Recover( FTIT_execution *FTI_Exec, FTIT_dataset *FTI_Data, FTIT_checkpoint *FTI_Ckpt );
int FTIFF_RecoverVar( int id, FTIT_execution *FTI_Exec, FTIT_dataset *FTI_Data, FTIT_checkpoint *FTI_Ckpt );
int FTIFF_UpdateDatastructVarFTIFF( FTIT_execution* FTI_Exec, 
        FTIT_dataset* FTI_Data, FTIT_configuration* FTI_Conf, 
        int pvar_idx );
int FTIFF_ReadDbFTIFF( FTIT_configuration *FTI_Conf, FTIT_execution *FTI_Exec, FTIT_checkpoint* FTI_Ckpt );
int FTIFF_GetFileChecksum( FTIFF_metaInfo *FTIFF_Meta, FTIT_checkpoint* FTI_Ckpt, int fd, char *checksum );
int FTIFF_WriteFTIFF(FTIT_configuration* FTI_Conf, FTIT_execution* FTI_Exec,
        FTIT_topology* FTI_Topo, FTIT_checkpoint* FTI_Ckpt,
        FTIT_dataset* FTI_Data);
int FTIFF_createHashesDbVarFTIFF( FTIT_execution* FTI_Exec, FTIT_dataset* FTI_Data );
int FTIFF_finalizeDatastructFTIFF( FTIT_execution* FTI_Exec, FTIT_dataset* FTI_Data );
int FTIFF_writeMetaDataFTIFF( FTIT_execution* FTI_Exec, int fd );
int FTIFF_CreateMetadata( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_dataset* FTI_Data, FTIT_configuration* FTI_Conf );
int FTIFF_CheckL1RecoverInit( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_checkpoint* FTI_Ckpt, FTIT_configuration* FTI_Conf );
int FTIFF_CheckL2RecoverInit( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_checkpoint* FTI_Ckpt, FTIT_configuration* FTI_Conf, int *exists);
int FTIFF_CheckL3RecoverInit( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_checkpoint* FTI_Ckpt, int* erased);
int FTIFF_CheckL4RecoverInit( FTIT_execution* FTI_Exec, FTIT_topology* FTI_Topo,
        FTIT_checkpoint* FTI_Ckpt);
void FTIFF_GetHashMetaInfo( unsigned char *hash, FTIFF_metaInfo *FTIFFMeta );
void FTIFF_GetHashdb( unsigned char *hash, FTIFF_db *db );
void FTIFF_GetHashdbvar( unsigned char *hash, FTIFF_dbvar *dbvar );
void FTIFF_SetHashChunk( FTIFF_dbvar *dbvar, FTIT_dataset* FTI_Data ); 
void FTIFF_PrintDataStructure( int rank, FTIT_execution* FTI_Exec, FTIT_dataset* FTI_Data );
#endif
