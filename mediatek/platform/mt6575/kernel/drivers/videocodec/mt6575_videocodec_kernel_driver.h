/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef __MT6573_MFLEXVIDEO_H__
#define __MT6573_MFLEXVIDEO_H__

#define MFV_IOC_MAGIC    'M'

//below is control message
#define MFV_TEST_CMD                            _IO(MFV_IOC_MAGIC,  0x00)
#define MFV_INIT_CMD                            _IO(MFV_IOC_MAGIC, 0x01)
#define MFV_DEINIT_CMD                          _IO(MFV_IOC_MAGIC, 0x02)
#define MFV_SET_CMD_CMD                         _IOW(MFV_IOC_MAGIC, 0x03, P_MFV_DRV_CMD_QUEUE_T)
#define MFV_SET_PWR_CMD                         _IOW(MFV_IOC_MAGIC, 0x04, HAL_POWER_T *)
#define MFV_SET_ISR_CMD                         _IOW(MFV_IOC_MAGIC, 0x05, HAL_ISR_T *)
#define MFV_ALLOC_MEM_CMD                       _IOW(MFV_IOC_MAGIC, 0x06, unsigned int)
#define MFV_FREE_MEM_CMD                        _IOW(MFV_IOC_MAGIC, 0x07, unsigned int)
#define MFV_MAKE_PMEM_TO_NONCACHED              _IOW(MFV_IOC_MAGIC, 0x08, unsigned int*)
#define MFV_ALLOC_INT_MEM_CMD                   _IOW(MFV_IOC_MAGIC, 0x09, VAL_INTMEM_T*)
#define MFV_FREE_INT_MEM_CMD                    _IOW(MFV_IOC_MAGIC, 0x0a, VAL_INTMEM_T*)
#define MT6575_VCODEC_WAITISR                   _IOW(MFV_IOC_MAGIC, 0x0b, HAL_POWER_T *)
#define MT6575_VCODEC_LOCKHW                    _IOW(MFV_IOC_MAGIC, 0x0d, VAL_HW_LOCK_T *)
#define MT6575_VCODEC_PMEM_FLUSH                _IOW(MFV_IOC_MAGIC, 0x10, HAL_POWER_T *)
#define MT6575_VCODEC_PMEM_CLEAN                _IOW(MFV_IOC_MAGIC, 0x11, HAL_POWER_T *)
#define MT6575_VCODEC_INC_SYSRAM_USER           _IOW(MFV_IOC_MAGIC, 0x13, VAL_UINT32_T *)
#define MT6575_VCODEC_DEC_SYSRAM_USER           _IOW(MFV_IOC_MAGIC, 0x14, VAL_UINT32_T *)
#define MT6575_VCODEC_INC_ENC_EMI_USER          _IOW(MFV_IOC_MAGIC, 0x15, VAL_UINT32_T *)
#define MT6575_VCODEC_DEC_ENC_EMI_USER          _IOW(MFV_IOC_MAGIC, 0x16, VAL_UINT32_T *)
#define MT6575_VCODEC_INC_DEC_EMI_USER          _IOW(MFV_IOC_MAGIC, 0x17, VAL_UINT32_T *)
#define MT6575_VCODEC_DEC_DEC_EMI_USER          _IOW(MFV_IOC_MAGIC, 0x18, VAL_UINT32_T *)
#define MT6575_VCODEC_INITHWLOCK                _IOW(MFV_IOC_MAGIC, 0x20, VAL_VCODEC_OAL_HW_REGISTER_T *)        
#define MT6575_VCODEC_DEINITHWLOCK              _IOW(MFV_IOC_MAGIC, 0x21, VAL_VCODEC_OAL_HW_REGISTER_T *)        
#define MT6575_VCODEC_ALLOC_NON_CACHE_BUFFER    _IOW(MFV_IOC_MAGIC, 0x22, VAL_MEMORY_T *) 
#define MT6575_VCODEC_FREE_NON_CACHE_BUFFER     _IOW(MFV_IOC_MAGIC, 0x23, VAL_MEMORY_T *) 
#define MT6575_VCODEC_SET_THREAD_ID             _IOW(MFV_IOC_MAGIC, 0x24, VAL_VCODEC_THREAD_ID_T *) 
#define MT6575_VCODEC_SET_SYSRAM_INFO           _IOW(MFV_IOC_MAGIC, 0x25, VAL_INTMEM_T *) 
#define MT6575_VCODEC_GET_SYSRAM_INFO           _IOW(MFV_IOC_MAGIC, 0x26, VAL_INTMEM_T *)
#define MT6575_VCODEC_INC_PWR_USER              _IOW(MFV_IOC_MAGIC, 0x27, HAL_POWER_T *)
#define MT6575_VCODEC_DEC_PWR_USER              _IOW(MFV_IOC_MAGIC, 0x28, HAL_POWER_T *)
#define MT6575_VCODEC_GET_CPU_LOADING_INFO      _IOW(MFV_IOC_MAGIC, 0x29, VAL_VCODEC_CPU_LOADING_INFO_T *) 


//#define MFV_GET_CACHECTRLADDR_CMD  _IOR(MFV_IOC_MAGIC, 0x06, int)

#endif //__MT6573_MFLEXVIDEO_H__
