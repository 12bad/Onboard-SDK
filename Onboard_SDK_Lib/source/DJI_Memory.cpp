/*
 * DJI_Pro_Rmu.cpp
 * Des:RMU,means Resource Management Unit, includes memory and session management
 *  Created on: 24 Aug, 2015
 *      Author: wuyuwei
 */


#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "DJI_Memory.h"
#include "DJI_API.h"

//! static pthread_mutex_t mmu_lock = PTHREAD_MUTEX_INITIALIZER; @note move to hw level

using namespace DJI::onboardSDK;

void DJI::onboardSDK::API::setupMMU()
{
    int i;
    MMU[0].tab_index = 0;
    MMU[0].usage_flag = 1;
    MMU[0].pmem = Static_Memory;
    MMU[0].mem_size = 0;
    for(i = 1 ; i < (MMU_TABLE_NUM - 1) ; i ++)
    {
        MMU[i].tab_index = i;
        MMU[i].usage_flag = 0;
    }
    MMU[MMU_TABLE_NUM - 1].tab_index = MMU_TABLE_NUM - 1;
    MMU[MMU_TABLE_NUM - 1].usage_flag = 1;
    MMU[MMU_TABLE_NUM - 1].pmem = Static_Memory + MEMORY_SIZE;
    MMU[MMU_TABLE_NUM - 1].mem_size = 0;
}

void freeMemory(MMU_Tab *mmu_tab)
{
    if(mmu_tab == (MMU_Tab*)0)
    {
        return;
    }
    if(mmu_tab->tab_index == 0 || mmu_tab->tab_index == (MMU_TABLE_NUM - 1))
    {
        return;
    }
    mmu_tab->usage_flag = 0;
}

MMU_Tab *DJI::onboardSDK::API::allocMemory(unsigned short size)
{
    unsigned int mem_used = 0;
    unsigned char i;
    unsigned char j = 0;
    unsigned char mmu_tab_used_num = 0;
    unsigned char mmu_tab_used_index[MMU_TABLE_NUM];

    unsigned int temp32;
    unsigned int temp_area[2] = {0xFFFFFFFF,0xFFFFFFFF};

    unsigned int record_temp32 = 0;
    unsigned char magic_flag = 0;

    if(size > PRO_PURE_DATA_MAX_SIZE || size > MEMORY_SIZE)
    {
        return (MMU_Tab *)0;
    }

    for(i = 0 ; i < MMU_TABLE_NUM ; i ++)
    {
        if(MMU[i].usage_flag == 1)
        {
            mem_used += MMU[i].mem_size;
            mmu_tab_used_index[mmu_tab_used_num ++] = MMU[i].tab_index;
        }
    }

    if(MEMORY_SIZE < (mem_used + size))
    {
        return (MMU_Tab *)0;
    }

    if(mem_used == 0)
    {
        MMU[1].pmem = MMU[0].pmem;
        MMU[1].mem_size = size;
        MMU[1].usage_flag = 1;
        return &MMU[1];
    }

    for(i = 0 ; i < (mmu_tab_used_num - 1) ; i ++)
    {
        for(j = 0; j < (mmu_tab_used_num - i - 1) ; j ++)
        {
            if(MMU[mmu_tab_used_index[j]].pmem >
                    MMU[mmu_tab_used_index[j + 1]].pmem)
            {
                mmu_tab_used_index[j + 1] ^= mmu_tab_used_index[j];
                mmu_tab_used_index[j] ^= mmu_tab_used_index[j + 1];
                mmu_tab_used_index[j + 1] ^= mmu_tab_used_index[j];
            }
        }
    }

    for(i = 0 ; i < (mmu_tab_used_num - 1) ; i ++)
    {
        temp32 = (unsigned int)(MMU[mmu_tab_used_index[i + 1]].pmem -
                MMU[mmu_tab_used_index[i]].pmem);

        if((temp32 - MMU[mmu_tab_used_index[i]].mem_size) >= size)
        {
            if(temp_area[1] > (temp32 - MMU[mmu_tab_used_index[i]].mem_size))
            {
                temp_area[0] = MMU[mmu_tab_used_index[i]].tab_index;
                temp_area[1] = temp32 - MMU[mmu_tab_used_index[i]].mem_size;
            }
        }

        record_temp32 += temp32 - MMU[mmu_tab_used_index[i]].mem_size;
        if(record_temp32 >= size && magic_flag == 0)
        {
            j = i;
            magic_flag = 1;
        }
    }

    if(temp_area[0] == 0xFFFFFFFF && temp_area[1] == 0xFFFFFFFF)
    {
        for(i = 0; i < j; i ++)
        {
            if(MMU[mmu_tab_used_index[i + 1]].pmem
                    >  (MMU[mmu_tab_used_index[i]].pmem +
                        MMU[mmu_tab_used_index[i]].mem_size))
            {
                memmove(MMU[mmu_tab_used_index[i]].pmem +
                        MMU[mmu_tab_used_index[i]].mem_size,
                        MMU[mmu_tab_used_index[i + 1]].pmem,
                        MMU[mmu_tab_used_index[i + 1]].mem_size);
                MMU[mmu_tab_used_index[i + 1]].pmem = MMU[mmu_tab_used_index[i]].pmem +
                        MMU[mmu_tab_used_index[i]].mem_size;
            }
        }

        for(i = 1 ; i < (MMU_TABLE_NUM - 1) ; i ++)
        {
            if(MMU[i].usage_flag == 0)
            {
                MMU[i].pmem =
                        MMU[mmu_tab_used_index[j]].pmem +
                        MMU[mmu_tab_used_index[j]].mem_size;

                MMU[i].mem_size = size;
                MMU[i].usage_flag = 1;
                return &MMU[i];
            }
        }
        return (MMU_Tab *)0;
    }

    for(i = 1 ; i < (MMU_TABLE_NUM - 1) ; i ++)
    {
        if(MMU[i].usage_flag == 0)
        {
            MMU[i].pmem = MMU[temp_area[0]].pmem +
                    MMU[temp_area[0]].mem_size;

            MMU[i].mem_size = size;
            MMU[i].usage_flag = 1;
            return &MMU[i];
        }
    }

    return (MMU_Tab *)0;
}

void DJI::onboardSDK::API::setupSession()
{
    unsigned int i;
    for(i = 0; i < SESSION_TABLE_NUM ; i ++)
    {
        CMDSessionTab[i].session_id = i;
        CMDSessionTab[i].usage_flag = 0;
        CMDSessionTab[i].mmu = (MMU_Tab *)NULL;
    }

    for(i = 0; i < (SESSION_TABLE_NUM - 1) ; i ++)
    {
        ACKSessionTab[i].session_id = i + 1;
        ACKSessionTab[i].session_status = ACK_SESSION_IDLE;
        ACKSessionTab[i].mmu = (MMU_Tab *)NULL;
    }
}

/* request a cmd session for sending cmd data
 * when arg session_id = 0/1, which means select session 0/1 to send cmd
 * otherwise set arg session_id = CMD_SESSION_AUTO (32), which means auto select a idle session id between 2~31.
 */

CMDSession* DJI::onboardSDK::API::allocSession(unsigned short session_id, unsigned short size)
{
    int i;
    MMU_Tab *mmu = NULL;

    if(session_id == 0 || session_id == 1)
    {
        if(this->CMDSessionTab[session_id].usage_flag == 0)
        {
            i = session_id;
        }
        else
        {
            /* session is busy */
            API_ERROR("%s:%d:ERROR,session %d is busy\n",__func__,__LINE__,session_id);
            return NULL;
        }
    }
    else
    {
        for(i = 2 ; i < SESSION_TABLE_NUM ; i ++)
        {
            if(CMDSessionTab[i].usage_flag == 0)
            {
                break;
            }
        }
    }
    if(i < 32 && CMDSessionTab[i].usage_flag == 0)
    {
        CMDSessionTab[i].usage_flag = 1;
        mmu = allocMemory(size);
        if(mmu == NULL)
        {
            CMDSessionTab[i].usage_flag = 0;
        }
        else
        {
            CMDSessionTab[i].mmu = mmu;
            return &CMDSessionTab[i];
        }
    }
    return NULL;
}

void DJI::onboardSDK::API:: freeSession(CMDSession *session)
{
    if(session->usage_flag == 1)
    {
        API_DEBUG("%s: session id %d\n",__func__,session->session_id);
        freeMemory(session->mmu);
        session->usage_flag = 0;
    }
}

ACKSession *DJI::onboardSDK::API::allocACK(unsigned short session_id, unsigned short size)
{
    MMU_Tab *mmu = NULL;
    if(session_id > 0 && session_id < 32)
    {
        if(ACKSessionTab[session_id - 1].mmu)
        {
            freeMemory(ACKSessionTab[session_id - 1].mmu);
        }
        mmu = allocMemory(size);
        if(mmu == NULL)
        {
        }
        else
        {
            ACKSessionTab[session_id - 1].mmu = mmu;
            return &ACKSessionTab[session_id - 1];
        }
    }
    return NULL;
}

void DJI::onboardSDK::API::freeACK(ACKSession *session)
{
    freeMemory(session->mmu);
}


