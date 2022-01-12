//-----------------------------/
#ifndef __DEF_SOCKET__
#define __DEF_SOCKET__
//------------------------------/

#define	PACKET_HEADER_SIZE		((int)sizeof(int) * 2)

typedef enum {
	emPdfsPacketHeader_1		= 0,
	emPdfsPacketHeader_2,

	emCountPdfsPacketHeader,
} enumPdfsPacketHeader;

typedef enum {
	// ���ε��� (TCP)
	emWM_SOCKET_ACCEPT_MN			= WM_USER+1,
	emWM_SOCKET_RECEIVE_MN,
	emWM_SOCKET_WARNING_MN,
	emWM_SOCKET_CLOSE_MN,
	// ���ε���(UDP) - ��������� ����
	emWM_SOCKET_RECEIVE_MN_UDP,					
	emWM_SOCKET_WARNING_MN_UDP,					
	emWM_SOCKET_CLOSE_MN_UDP,					

	// ����ǰ������(TCP)
	emWM_SOCKET_ACCEPT_MR,
	emWM_SOCKET_RECEIVE_MR,
	emWM_SOCKET_WARNING_MR,
	emWM_SOCKET_CLOSE_MR,
	// ����ǰ������(UDP)
	emWM_SOCKET_RECEIVE_MR_UDP,					
	emWM_SOCKET_WARNING_MR_UDP,					
	emWM_SOCKET_CLOSE_MR_UDP,					

	// ����������(TCP)
	emWM_SOCKET_ACCEPT_MO,
	emWM_SOCKET_RECEIVE_MO,
	emWM_SOCKET_WARNING_MO,
	emWM_SOCKET_CLOSE_MO,
	// ����������(UDP)
	emWM_SOCKET_RECEIVE_MO_UDP,					
	emWM_SOCKET_WARNING_MO_UDP,					
	emWM_SOCKET_CLOSE_MO_UDP,

	// ����Ž��(TCP)
	emWM_SOCKET_ACCEPT_DF,
	emWM_SOCKET_RECEIVE_DF,
	emWM_SOCKET_WARNING_DF,
	emWM_SOCKET_CLOSE_DF,
	// ����Ž��(UDP)
	emWM_SOCKET_RECEIVE_DF_UDP,					
	emWM_SOCKET_WARNING_DF_UDP,					
	emWM_SOCKET_CLOSE_DF_UDP,

	//////////////////////////////////////////////////////////////////////////

	emWM_SOCKET_RECEIVE_DEM,
	emWM_SOCKET_WARNING_DEM,
	emWM_SOCKET_CLOSE_DEM,

	emWM_Socket_Receive_CDMA,
	emWM_Socket_Warning_CDMA,
	emWM_Socket_Close_CDMA,

	emWM_Diagram_Receive_CDMA,
	emWM_Diagram_Warning_CDMA,
	emWM_Diagram_Close_CDMA,

	emWM_Broadcast_Receive_PDFS,
	emWM_Broadcast_Warning_PDFS,
	emWM_Broadcast_Close_PDFS,

	emWM_CDMA_Connect_PDFS,
	emWM_CDMA_Socket_Accept,

	emWM_Socket_Receive_Simulator,
	emWM_Socket_Warning_Simulator,
	emWM_Socket_Close_Simulator,

	//////////////////////////////////////////////////////////////////////////
	// IQ������ ���ۿ�(TCP)
	emWM_SOCKET_ACCEPT_IQ,
	emWM_SOCKET_RECEIVE_IQ,
	emWM_SOCKET_WARNING_IQ,
	emWM_SOCKET_CLOSE_IQ,
	//////////////////////////////////////////////////////////////////////////

	// FFT ���ۿ�(TCP)
	emWM_SOCKET_ACCEPT_FFT,
	emWM_SOCKET_RECEIVE_FFT,
	emWM_SOCKET_WARNING_FFT,
	emWM_SOCKET_CLOSE_FFT,

	// �ذ�����(TCP)
	emWM_SOCKET_ACCEPT_SFMS,
	emWM_SOCKET_RECEIVE_SFMS,
	emWM_SOCKET_WARNING_SFMS,
	emWM_SOCKET_CLOSE_SFMS,

	emWM_SOCKET_ACCEPT_SFMS_FFT,
	emWM_SOCKET_RECEIVE_SFMS_FFT,
	emWM_SOCKET_WARNING_SFMS_FFT,
	emWM_SOCKET_CLOSE_SFMS_FFT,

	emCountSocketEventTag,
} enumSocketEventTag;

typedef enum {
	emPdfsGetSystemInfo_TAG			= 0,			
	emPdfsCheckUserInfo_TAG,						
	emPdfsManagementUser_TAG,						
	emPdfsUpdateEnvInfo_TAG,						
	emPdfsSaveRemoteResult_TAG,						
	emPdfsSaveLocalResult_TAG,						
	emPdfsLoadRemoteResult_TAG,						
	emPdfsLoadLocalResult_TAG,						
	emPdfsLoadResultEnd_TAG,						
	emDummyDatabaseMessageEnd_TAG,					
} enumPdfsDatabaseMessageTag;

typedef enum {
	emPdfsRadioCommand_TAG			= 0,			
	emDummyRadioMessageEnd_TAG,
} enumPdfsRadioMessageTag;

typedef enum {
	emPdfsDeviceMeasureEnv_TAG		= 0,			
	emPdfsDeviceMeasureStop_TAG,					
	emDummyDeviceMessageEnd_TAG,
} enumPdfsDeviceMessageTag;

typedef enum {
	emPdfsMesaureTime_GPS_TAG		= 0,
	emPdfsLatitude_GPS_TAG,
	emPdfsLongitude_GPS_TAG,
	emPdfsState_GPS_TAG,
	emPdfsSatelliteNum_GPS_TAG,
	emPdfsHDOP_GPS_TAG,
	emPdfsAntennaHeight_GPS_TAG,
	emPdfsMesaureDay_GPS_TAG,
	emPdfsMesaureMonth_GPS_TAG,
	emPdfsMesaureYear_GPS_TAG,
	emPdfsTrueBearing_GPS_TAG,
	emPdfsMagneticBearing_GPS_TAG,
	emPdfsGroundSpeedKNT_GPS_TAG,
	emPdfsGroundSpeedKMH_GPS_TAG,
	emPdfsMode_GPS_TAG,
	emPdfsCompass_Tag,
} enumPdfsDeviceLogTag;

typedef enum {
	emPdfsAntenna_GPS_TAG		= 0,				
	emPdfsAntenna_DGPS_TAG,							
	emPdfsAntenna_DF_TAG,							
	emDummyAntennaMessageEnd_TAG,
} enumPdfsAntennaMessageTag;



#if !defined(DEF_ENUM_PACKET_TAG) // ###################################

typedef enum _enumPacketTag
{
    PID_PACKET_TOP	                    = 0,	//                                              
    PID_SYS_REBOOT,                             // �ý��� �����                                
                                                                                                
    //--------------------------------------------                                              
    // ���� ���� - ���� ���� ����                                                               
    //--------------------------------------------                                              
    PID_MON_CONNECT_MN,                         // ���ε��� ����                                
    PID_MON_DISCONNECT_MN,                      // ���ε��� ���� ����                           
                                                                                                
    //--------------------------------------------                                              
    // ����ǰ�� ���� ����                                                                       
    //--------------------------------------------                                              
    PID_MON_CONFIRM_MR,                         // ����ǰ�� ���� ����Ȯ��                       
    PID_MON_CONNECT_MR,                         // ����ǰ�� ���� ����                           
    PID_MON_DISCONNECT_MR,                      // ����ǰ�� ���� ���� ����                      
    PID_MON_INIT_MR,                            // ����ǰ�� ���� �ʱ�ȭ                         
                                                                                                
    //--------------------------------------------                                              
    // �ҹ����� ���� ���� - NOT USED -                                                          
    //--------------------------------------------                                              
    PID_MON_CONFIRM_MI,                         // �ҹ����� ���� ����Ȯ��                       
    PID_MON_CONNECT_MI,                         // �ҹ����� ���� ����                           
    PID_MON_DISCONNECT_MI,                      // �ҹ����� ���� ���� ����                      
    PID_MON_INIT_MI,                            // �ҹ����� ���� �ʱ�ȭ                         
                                                                                                
    //--------------------------------------------                                              
    // ������ ����                                                                              
    //--------------------------------------------                                              
    PID_MON_CONFIRM_MO,                         // ����ȯ������ ���� ����Ȯ��                   
    PID_MON_CONNECT_MO,                         // ����ȯ������ ���� ����                       
    PID_MON_DISCONNECT_MO,                      // ����ȯ������ ���� ���� ����                  
    PID_MON_INIT_MO,                            // ����ȯ������ ���� �ʱ�ȭ                     
                                                                                                
    //--------------------------------------------                                              
    // ���� �Ķ���� ����                                                                       
    //--------------------------------------------                                              
    PID_MEASURE_PARAM_MR_DT,                    // ����ǰ�� ��������                            
    PID_MEASURE_PARAM_MR_SE,                    // ���Ľ���Ʈ������                             
    PID_MEASURE_PARAM_MR_AT,                    // �ִ���Ź�����                               
    PID_MEASURE_PARAM_MR_SP_S,                  // ��ǻ�������                               
    PID_MEASURE_PARAM_MR_SP_H,                  // ����������                                   
    // ����ǰ�� - ä��                                                                          
    PID_MEASURE_PARAM_MR_CH_S,                  // ä������ ���� ����                           
    PID_MEASURE_PARAM_MR_CH_D,                  // ä������ ������                              
    PID_MEASURE_PARAM_MR_CH_E,                  // ä������ ���� ����                           
    // �ҹ����� - ä��                                                                          
    PID_MEASURE_PARAM_MI_CH_S,                  // ä������ ���� ����                           
    PID_MEASURE_PARAM_MI_CH_D,                  // ä������ ������                              
    PID_MEASURE_PARAM_MI_CH_E,                  // ä������ ���� ����                           
    // �ҹ����� - �뿪                                                                          
    PID_MEASURE_PARAM_MI_BW,                    // �뿪����                                     
    // ������ - ä��                                                                            
    PID_MEASURE_PARAM_MO_CH_S,                  // ä������ ���� ����                           
    PID_MEASURE_PARAM_MO_CH_D,                  // ä������ ������                              
    PID_MEASURE_PARAM_MO_CH_E,                  // ä������ ���� ����                           
    // ������ - �뿪                                                                            
    PID_MEASURE_PARAM_MO_BW,                    // �뿪����                                     
                                                                                                
    // MR/MI/MO �ڵ��Ӱ跹�� ������	                                                            
    PID_MEASURE_PARAM_MR_TH,                    // �Ӱ跹��                                     
    PID_MEASURE_PARAM_MI_TH,                    // �Ӱ跹��                                     
    PID_MEASURE_PARAM_MI_TH_RESEND, 	        // �Ӱ跹���� 1000�ľ� �̾�ޱ�                 
    PID_MEASURE_PARAM_MO_TH,                    // �Ӱ跹��                                     
                                                                                                
    PID_MEASURE_PARAM_MR_ENV,                   // ����ǰ�� ȯ������                            
    PID_MEASURE_PARAM_MI_ENV,                   // �ҹ����� ȯ������                            
    PID_MEASURE_PARAM_MO_ENV,                   // ����ȯ������ ȯ������                        
                                                                                                
    //--------------------------------------------                                              
    // ���� ��� ����/���� �ױ�                                                                 
    //--------------------------------------------                                              
    PID_MEASURE_PARAM_MR_DT_R,                  // ����ǰ�� �������� ����                       
    PID_MEASURE_PARAM_MR_SE_R,                  // ���Ľ���Ʈ������ ����                        
    PID_MEASURE_PARAM_MR_AT_R,                  // �ִ���Ź����� ���� [������]                 
    PID_MEASURE_PARAM_MR_SP_S_R,                // ��ǻ������� ����                          
    PID_MEASURE_PARAM_MR_SP_H_R,                // ���������� ����                              
    // ����ǰ�� - ä�� -                                                                        
    PID_MEASURE_PARAM_MR_CH_S_R,                // ����ǰ�� ä������ ���� ���� ����             
    PID_MEASURE_PARAM_MR_CH_D_R,                // ����ǰ�� ä������ ������ ����                
    PID_MEASURE_PARAM_MR_CH_E_R,                // ����ǰ�� ä������ ���� ���� ����             
    // �ҹ����� - ä�� - ������			                                                        
    PID_MEASURE_PARAM_MI_CH_S_R,                // �ҹ����� ä������ ���� ���� ����             
    PID_MEASURE_PARAM_MI_CH_D_R,                // �ҹ����� ä������ ������ ����                
    PID_MEASURE_PARAM_MI_CH_E_R,                // �ҹ����� ä������ ���� ���� ����             
    PID_MEASURE_PARAM_MI_BW_R,                  // �ҹ����� �뿪���� ����                       
	// ����ȯ������ - ä�� -	                                                                
    PID_MEASURE_PARAM_MO_CH_S_R,                // ����ȯ������ ä������ ���� ���� ����         
    PID_MEASURE_PARAM_MO_CH_D_R,                // ����ȯ������ ä������ ������ ����            
    PID_MEASURE_PARAM_MO_CH_E_R,                // ����ȯ������ ä������ ���� ���� ����         
    PID_MEASURE_PARAM_MO_BW_R,                  // ����ȯ������ �뿪���� ����                   
                                                                                                
    //���� ����                                                                                 
    PID_MEASURE_START_MR,                       	                                            
    PID_MEASURE_START_MI,                                                                       
    PID_MEASURE_START_MO,                                                                       
    PID_MEASURE_START_ME,                                                                       
    PID_MEASURE_START_MP,                                                                       
    //���� ����                                                                                 
    PID_MEASURE_STOP_MR,                                                                        
    PID_MEASURE_STOP_MI,                                                                        
    PID_MEASURE_STOP_MO,                                                                        
    PID_MEASURE_STOP_ME,                                                                        
    PID_MEASURE_STOP_MP,                                                                        
    //���� �Ϸ�                                                                                 
    PID_MEASURE_COMPLETE_MR,                                                                    
    PID_MEASURE_COMPLETE_MI,                                                                    
    PID_MEASURE_COMPLETE_MO,                                                                    
    PID_MEASURE_COMPLETE_ME,                                                                    
    PID_MEASURE_COMPLETE_MP,                                                                    
                                                                                                
    // ������ ����                                                                              
    PID_MEASURE_START_MR_DUMMY,                                                                 
    PID_MEASURE_START_MI_DUMMY,                                                                 
    PID_MEASURE_START_MO_DUMMY,                                                                 
    PID_MEASURE_START_ME_DUMMY,                                                                 
    PID_MEASURE_START_MP_DUMMY,                                                                 
    // ������ ����                                                                              
    PID_MEASURE_STOP_MR_DUMMY,                                                                  
    PID_MEASURE_STOP_MI_DUMMY,                                                                  
    PID_MEASURE_STOP_MO_DUMMY,                                                                  
    PID_MEASURE_STOP_ME_DUMMY,                                                                  
    PID_MEASURE_STOP_MP_DUMMY,                                                                  
    // ������ �Ϸ�                                                                              
    PID_MEASURE_COMPLETE_MR_DUMMY,                                                              
    PID_MEASURE_COMPLETE_MI_DUMMY,                                                              
    PID_MEASURE_COMPLETE_MO_DUMMY,                                                              
    PID_MEASURE_COMPLETE_ME_DUMMY,                                                              
    PID_MEASURE_COMPLETE_MP_DUMMY,                                                              
    // ����Ʈ�� ������                                                                          
    PID_MEASURE_FFT_MR_INIT_ENV,                                                                
    PID_MEASURE_FFT_MI_INIT_ENV,                                                                
    PID_MEASURE_FFT_MO_INIT_ENV,                                                                
    PID_MEASURE_FFT_MR_CURRENT,                                                                 
    PID_MEASURE_FFT_MI_CURRENT,                                                                 
    PID_MEASURE_FFT_MO_CURRENT,                                                                 
    // ���� ���                                                                                
    PID_MEASURE_RESULT_MR,                                                                      
    PID_MEASURE_RESULT_MI,                                                                      
    PID_MEASURE_RESULT_MO,                                                                      
    PID_MEASURE_RESULT_ME,                                                                      
    PID_MEASURE_RESULT_MP,                                                                      
    PID_MEASURE_RESULT_AT,                                                                      
    PID_MEASURE_RESULT_TH,                                                                      
    // 1ȸ ��ĵ �Ϸ�		                                                                    
    PID_MEASURE_SCAN_1_COMPLETE_ME,                                                             
    PID_MEASURE_SCAN_1_COMPLETE_MI,                                                             
    PID_MEASURE_SCAN_1_COMPLETE_MO,                                                             
                                                                                                
    // �ҹ�����Ž�� - �뿪������ �㰡���ļ��뿪 �����ϱ� ����                                   
    PID_LICENSE_BAND_INFO,                      // �㰡�뿪 ���� �۽�                           
    PID_LICENSE_BAND_INFO_R,                    // �㰡�뿪 ���� ���� ����                      
                                                                                                
    //--------------------------------------------                                              
    // ������� �������� ����                                                                   
    //--------------------------------------------                                              
    PID_DIRECT_RX_INIT_S,                       // ���ű� �ʱ�ȭ                                
    PID_DIRECT_RX_MEASURE_TIME_MR_S,            // ����ǰ�� �����ð� ����                       
    PID_DIRECT_RX_MEASURE_TIME_MR_R,            // ����ǰ�� �����ð� ���� ����                  
    PID_DIRECT_RX_MEASURE_TIME_FSCAN_S,         // FSCAN �����ð� ����                          
    PID_DIRECT_RX_MEASURE_TIME_FSCAN_R,         // FSCAN �����ð� ���� ����                     
    PID_DIRECT_RX_MEASURE_TIME_MSCAN_S,         // MSCAN �����ð� ����                          
    PID_DIRECT_RX_MEASURE_TIME_MSCAN_R,         // MSCAN �����ð� ���� ����                     
    PID_DIRECT_RX_MEASURE_COUNT_MR_S,           // ����ǰ�� ����Ƚ�� ����                       
    PID_DIRECT_RX_MEASURE_COUNT_MR_R,           // ����ǰ�� ����Ƚ�� ���� ����                  
    PID_DIRECT_RX_MEASURE_COUNT_FSCAN_S,        // FSCAN ����Ƚ�� ����                          
    PID_DIRECT_RX_MEASURE_COUNT_FSCAN_R,        // FSCAN ����Ƚ�� ���� ����                     
    PID_DIRECT_RX_MEASURE_COUNT_MSCAN_S,        // MSCAN ����Ƚ�� ����                          
    PID_DIRECT_RX_MEASURE_COUNT_MSCAN_R,        // MSCAN ����Ƚ�� ���� ����                     
    PID_DIRECT_RX_MODE_S,                       // ���ű���[FFM,FSCAN,MSCAN] ����             
    PID_DIRECT_RX_MODE_Q,                       // ���ű���[FFM,FSCAN,MSCAN] ������ ��û      
    PID_DIRECT_RX_MODE_R,                       // ���ű���[FFM,FSCAN,MSCAN] ������ ����      
    PID_DIRECT_RX_FREQ_S,                       // ���ű� ���ļ� ����                           
    PID_DIRECT_RX_FREQ_Q,                       // ���ű� ���ļ� ������ ��û                    
    PID_DIRECT_RX_FREQ_R,                       // ���ű� ���ļ� ������ ����                    
    PID_DIRECT_RX_DEMOD_S,                      // ���ű� �������� ����                         
    PID_DIRECT_RX_DEMOD_Q,                      // ���ű� �������� ������ ��û                  
    PID_DIRECT_RX_DEMOD_R,                      // ���ű� �������� ������ ����                  
    PID_DIRECT_RX_BW_S,                         // ���ű� �뿪�� ����                           
    PID_DIRECT_RX_BW_Q,                         // ���ű� �뿪�� ������ ��û                    
    PID_DIRECT_RX_BW_R,                         // ���ű� �뿪�� ������ ����                    
    PID_DIRECT_RX_SQUELCH_STATE_S,              // ���ű� ����ġ ����                           
    PID_DIRECT_RX_SQUELCH_STATE_Q,              // ���ű� ����ġ ������ ��û                    
    PID_DIRECT_RX_SQUELCH_STATE_R,              // ���ű� ����ġ ������ ����                    
    PID_DIRECT_RX_THRESHOLD_S,                  // ���ű� �Ӱ跹�� ����                         
    PID_DIRECT_RX_THRESHOLD_Q,                  // ���ű� �Ӱ跹�� ������ ��û                  
    PID_DIRECT_RX_THRESHOLD_R,                  // ���ű� �Ӱ跹�� ������ ����                  
    PID_DIRECT_RX_AFC_STATE_S,                  // ���ű� AFC ON/OFF ����                       
    PID_DIRECT_RX_AFC_STATE_Q,                  // ���ű� AFC ON/OFF ������ ��û                
    PID_DIRECT_RX_AFC_STATE_R,                  // ���ű� AFC ON/OFF ������ ����                
    PID_DIRECT_RX_ATT_MODE_T,                   // ���ű� ATT �ڵ�/���� ���� [������]           
    PID_DIRECT_RX_ATT_MODE_S,                   // ���ű� ATT �ڵ�/���� ����                    
    PID_DIRECT_RX_ATT_MODE_Q,                   // ���ű� ATT �ڵ�/���� ������ ��û             
    PID_DIRECT_RX_ATT_MODE_R,                   // ���ű� ATT �ڵ�/���� ������ ����             
    PID_DIRECT_RX_ATT_VALUE_T,                  // ���ű� ATT �� ���� [������]                  
    PID_DIRECT_RX_ATT_VALUE_S,	                // ���ű� ATT �� ����                           
    PID_DIRECT_RX_ATT_VALUE_Q,	                // ���ű� ATT �� ������ ��û                    
    PID_DIRECT_RX_ATT_VALUE_R,	                // ���ű� ATT �� ������ ����                    
    PID_DIRECT_RX_RF_MODE_S,	                // ���ű� NOR/LNA/DISTORTION ����               
    PID_DIRECT_RX_RF_MODE_Q,	                // ���ű� NOR/LNA/DISTORTION ������ ��û        
    PID_DIRECT_RX_RF_MODE_R,	                // ���ű� NOR/LNA/DISTORTION ������ ����        
    PID_DIRECT_RX_GAIN_SEL_T,	                // ���ű� GAIN �ڵ�/���� ���� [������]          
    PID_DIRECT_RX_GAIN_SEL_S,	                // ���ű� GAIN �ڵ�/���� ����                   
    PID_DIRECT_RX_GAIN_SEL_Q,	                // ���ű� GAIN �ڵ�/���� ������ ��û            
    PID_DIRECT_RX_GAIN_SEL_R,	                // ���ű� GAIN �ڵ�/���� ������ ����            
    PID_DIRECT_RX_GAIN_VALUE_T,                 // ���ű� GAIN �� ���� [������]                 
    PID_DIRECT_RX_GAIN_VALUE_S,                 // ���ű� GAIN �� ����                          
    PID_DIRECT_RX_GAIN_VALUE_Q,                 // ���ű� GAIN �� ������ ��û                   
    PID_DIRECT_RX_GAIN_VALUE_R,                 // ���ű� GAIN �� ������ ����                   
    PID_DIRECT_RX_LEVEL_INDICATE_SEL_S,         // ���ŷ���[AVG/PEAK/FAST/RMS] ����             
    PID_DIRECT_RX_LEVEL_INDICATE_SEL_Q,         // ���ŷ���[AVG/PEAK/FAST/RMS] ������ ��û      
    PID_DIRECT_RX_LEVEL_INDICATE_SEL_R,         // ���ŷ���[AVG/PEAK/FAST/RMS] ������ ����      
    PID_DIRECT_RX_VOLUME_S,                     // ���ű� ���� ����                             
    PID_DIRECT_RX_VOLUME_Q,                     // ���ű� ���� ������ ��û                      
    PID_DIRECT_RX_VOLUME_R,                     // ���ű� ���� ������ ����                      
    PID_DIRECT_RX_CALFILTER_STATE_S,            // ���ű� CAL FILTER[ON/OFF] ����               
    PID_DIRECT_RX_CALFILTER_STATE_Q,            // ���ű� CAL FILTER[ON/OFF] ������ ��û        
    PID_DIRECT_RX_CALFILTER_STATE_R,            // ���ű� CAL FILTER[ON/OFF] ������ ����        
    PID_DIRECT_RX_FPDP_MODE_S,                  // ���ű� FPDP [BIT32_FLAG] ����                
    PID_DIRECT_RX_FPDP_MODE_Q,                  // ���ű� FPDP [BIT32_FLAG] ������ ��û         
    PID_DIRECT_RX_FPDP_MODE_R,                  // ���ű� FPDP [BIT32_FLAG] ������ ����         
    PID_DIRECT_RX_LEVEL_Q,                      // ���ű� ���ŷ��� ��û                         
    PID_DIRECT_RX_LEVEL_R,                      // ���ű� ���ŷ��� ����                         
    PID_DIRECT_RX_SELECT_DEVICE_S,              // ���ű� [1/2/3/4/5] ����                      
    PID_DIRECT_RX_SELECT_DEVICE_R,              // ���ű� [1/2/3/4/5] ���� ����                 
                                                                                                
    //--------------------------------------------                                              
    // ���׳�/���ܱ�/����ġ ���� ���� - ������                                                  
    //--------------------------------------------                                              
    PID_DIRECT_ANT_CH_SEL_S,                                                                    
    PID_DIRECT_ANT_CH_SEL_R,                                                                    
    PID_DIRECT_ANT_DIR_ANGLE_S,                                                                 
    PID_DIRECT_ANT_DIR_ANGLE_R,                                                                 
    PID_DIRECT_ANT_DIR_ANGLE_E,                                                                 
    PID_DIRECT_ANT_DIR_STOP_S,                                                                  
    PID_DIRECT_ANT_DIR_STOP_R,                                                                  
    PID_DIRECT_ANT_ROTATE_CW_S,                                                                 
    PID_DIRECT_ANT_ROTATE_CW_R,                                                                 
    PID_DIRECT_ANT_ROTATE_CCW_S,                                                                
    PID_DIRECT_ANT_ROTATE_CCW_R,                                                                
    PID_DIRECT_ANT_STATE_QUERY_S,                                                               
    PID_DIRECT_ANT_STATE_QUREY_R,                                                               
    PID_DIRECT_ANT_PRESELECT_S,                                                                 
    PID_DIRECT_ANT_PRESELECT_R,                                                                 
    PID_DIRECT_ANT_CYCLE_ATTRIB_S,                                                              
    PID_DIRECT_ANT_CYCLE_ATTRIB_R,                                                              
    PID_DIRECT_ANT_CYCLE_ATTRIB_Q,                                                              
                                                                                                
    //--------------------------------------------                                              
    // �ý��� ���� ���� - ����                                                                  
    //--------------------------------------------                                              
    PID_SYS_MON_CONFIG_SET_Q,                   // ������ ���׳�Ÿ��/��� ����                  
    PID_SYS_MON_CONFIG_SET_R,                   // ������ ���׳�Ÿ��/��� ������ ����           
                                                                                                
    //--------------------------------------------                                              
    // �ý��� ���� ���� - ����                                                                  
    //--------------------------------------------                                              
    PID_SYS_MON_CAL_INIT_S,                     // ������ �ý��ۺ��� �ʱ����̺� ��û          
    PID_SYS_MON_CAL_INIT_R,                     // ������ �ý��ۺ��� �ʱ����̺� ����          
    PID_SYS_MON_CAL_INIT_E,                     // ������ �ý��ۺ��� �ʱ����̺� ���� ����     
    PID_SYS_MON_CAL_MEASURE_S,                  // ������ �ý��ۺ��� ����                       
    PID_SYS_MON_CAL_MEASURE_R,                  // ������ �ý��ۺ��� ����� ����                
    PID_SYS_MON_CAL_MEASURE_E,                  // ������ �ý��ۺ��� ����                       
    PID_SYS_MON_CAL_MEASURE_P,            	    // ������ �ý��ۺ����� ������Ʈ�� �ݿ�	        
    PID_SYS_MON_CAL_MEASURE_C,                  // ������ �ý��ۺ��� ��� ��û                  
    		                                                                                    
    PID_SYS_MON_ANT_FACTOR_INIT_S,              // ������ ���׳� Ÿ�Ժ� ���Ͱ� ���� ����        
    PID_SYS_MON_ANT_FACTOR_INIT_R,           	// ������ ���׳� Ÿ�Ժ� ���Ͱ� ���� ����        
    PID_SYS_MON_ANT_FACTOR_INIT_E,          	// ������ ���׳� Ÿ�Ժ� ���Ͱ� ���� ����        
                                                                                                
    //--------------------------------------------                                              
    // �ڱ����� ����                                                                            
    // ���� : (INT-R == 0 ���� / INT-R < 0 ����)                                                
    //--------------------------------------------                                              
    PID_DEVICE_STATUS_RX1_Q,                    // ���ű�#1 �ڱ����� ��û                       
    PID_DEVICE_STATUS_RX1_R,                    // ���ű�#1 �ڱ����� ����                       
    PID_DEVICE_STATUS_RX2_Q,                    // ���ű�#2 �ڱ����� ��û                       
    PID_DEVICE_STATUS_RX2_R,                    // ���ű�#2 �ڱ����� ����                       
    PID_DEVICE_STATUS_RX3_Q,                    // ���ű�#3 �ڱ����� ��û                       
    PID_DEVICE_STATUS_RX3_R,                    // ���ű�#3 �ڱ����� ����                       
    PID_DEVICE_STATUS_RX4_Q,                    // ���ű�#4 �ڱ����� ��û                       
    PID_DEVICE_STATUS_RX4_R,                    // ���ű�#4 �ڱ����� ����                       
    PID_DEVICE_STATUS_RX5_Q,                    // ���ű�#5 �ڱ����� ��û                       
    PID_DEVICE_STATUS_RX5_R,                    // ���ű�#5 �ڱ����� ����            	        
    PID_DEVICE_STATUS_ANT_Q,                    // ���׳� ����� �ڱ����� ��û                  
    PID_DEVICE_STATUS_ANT_R,                    // ���׳� ����� �ڱ����� ����                  
    PID_DEVICE_STATUS_RFC_Q,                    // ���׳� ��ȣ�й�� �ڱ����� ��û              
    PID_DEVICE_STATUS_RFC_R,                    // ���׳� ��ȣ�й�� �ڱ����� ����              
    PID_DEVICE_STATUS_DSP_Q,                    // DSP �ڱ����� ��û                            
    PID_DEVICE_STATUS_DSP_R,                    // DSP �ڱ����� ����                            
    PID_DEVICE_STATUS_ATR_Q,                    // ���׳�ȸ���� �ڱ����� ��û                   
    PID_DEVICE_STATUS_ATR_R,                    // ���׳�ȸ���� �ڱ����� ����                   
                                                                                                
    //--------------------------------------------                                              
    // �ý��� �� ����                                                                         
    // ���� : (�� ��� ���ǵ� ���� ��Ʈ 5��)                                                  
    //--------------------------------------------                                              
    PID_DEVICE_DETAIL_STATUS_RX1_Q,             // ���ű�#1 ������ ��û                       
    PID_DEVICE_DETAIL_STATUS_RX1_R,             // ���ű�#1 ������ ����                       
    PID_DEVICE_DETAIL_STATUS_RX2_Q,             // ���ű�#2 ������ ��û                       
    PID_DEVICE_DETAIL_STATUS_RX2_R,             // ���ű�#2 ������ ����                       
    PID_DEVICE_DETAIL_STATUS_RX3_Q,             // ���ű�#3 ������ ��û                       
    PID_DEVICE_DETAIL_STATUS_RX3_R,             // ���ű�#3 ������ ����                       
    PID_DEVICE_DETAIL_STATUS_RX4_Q,             // ���ű�#4 ������ ��û                       
    PID_DEVICE_DETAIL_STATUS_RX4_R,             // ���ű�#4 ������ ����                       
    PID_DEVICE_DETAIL_STATUS_RX5_Q,             // ���ű�#5 ������ ��û                       
    PID_DEVICE_DETAIL_STATUS_RX5_R,             // ���ű�#5 ������ ����            	        
    PID_DEVICE_DETAIL_STATUS_ANT_Q,             // ���׳� ����� ������ ��û                  
    PID_DEVICE_DETAIL_STATUS_ANT_R,             // ���׳� ����� ������ ����                  
    PID_DEVICE_DETAIL_STATUS_RFC_Q,             // ���׳� ��ȣ�й�� ������ ��û              
    PID_DEVICE_DETAIL_STATUS_RFC_R,             // ���׳� ��ȣ�й�� ������ ����              
    PID_DEVICE_DETAIL_STATUS_DSP_Q,             // DSP ������ ��û                            
    PID_DEVICE_DETAIL_STATUS_DSP_R,             // DSP ������ ����                            
    PID_DEVICE_DETAIL_STATUS_ATR_Q,             // ���׳�ȸ���� ������ ��û                   
    PID_DEVICE_DETAIL_STATUS_ATR_R,             // ���׳�ȸ���� ������ ����                   
                                                                                                
    //--------------------------------------------                                              
    // �������� ���� ���� ����                                                                  
    //--------------------------------------------                                              
    PID_DEMOD_MANUAL_OPERATE_SET_S,                                                             
    PID_DEMOD_MANUAL_OPERATE_SET_R,                                                             
    PID_DEMOD_MANUAL_OPERATE_REC_S,                                                             
    PID_DEMOD_MANUAL_OPERATE_REC_R,                                                             
    PID_DEMOD_MANUAL_OPERATE_SET_END_S,                                                         
    PID_DEMOD_MANUAL_OPERATE_SET_END_R,                                                         
    PID_DEMOD_MANUAL_OPERATE_REC_END_S,                                                         
    PID_DEMOD_MANUAL_OPERATE_REC_END_R,                                                         
    		                                                                                    
    // ���ű⺰ [AFC]                                                                           
    PID_CURRENT_FREQ_RX1_Q,                                                                     
    PID_CURRENT_FREQ_RX1_R,                     // ���� �������� ���ļ� [UDP ����] ���ۿ�       
    PID_CURRENT_FREQ_RX2_Q,                                                                     
    PID_CURRENT_FREQ_RX2_R,                                                                     
    PID_CURRENT_FREQ_RX3_Q,                                                                     
    PID_CURRENT_FREQ_RX3_R,                                                                     
    PID_CURRENT_FREQ_RX4_Q,                                                                     
    PID_CURRENT_FREQ_RX4_R,                                                                     
    PID_CURRENT_FREQ_RX5_Q,                                                                     
    PID_CURRENT_FREQ_RX5_R,                                                                     
    // ���ű⺰ ��ȣ���� ó��                                                                   
    PID_CURRENT_LEVEL_RX1_Q,                                                                    
    PID_CURRENT_LEVEL_RX1_R,					// ����ǰ������: ���������� ��� ä���Ŀ�(double������) ���� ���
    PID_CURRENT_LEVEL_RX2_Q,                                                                    
    PID_CURRENT_LEVEL_RX2_R,                                                                    
    PID_CURRENT_LEVEL_RX3_Q,                                                                    
    PID_CURRENT_LEVEL_RX3_R,                                                                    
    PID_CURRENT_LEVEL_RX4_Q,                                                                    
    PID_CURRENT_LEVEL_RX4_R,                                                                    
    PID_CURRENT_LEVEL_RX5_Q,                                                                    
    PID_CURRENT_LEVEL_RX5_R,                                                                    
                                                                                                
    PID_MON_ERROR,								// �������� ������ ��� [ MON --> OP ] ����     
                                                                                                
    //--------------------------------------------                                              
    // ��밨�� - NOT USED -                                                                    
    //--------------------------------------------                                              
    PID_MON_CONFIRM_MA,                                                                         
    PID_MON_CONNECT_MA,                                                                         
    PID_MON_DISCONNECT_MA,                                                                      
    PID_MON_INIT_MA,                                                                            
    PID_MEASURE_COMPLETE_MA,                                                                    
    PID_MEASURE_PARAM_MA_DT,                                                                          
    PID_MEASURE_PARAM_MA_BW_S,                                                                        
    PID_MEASURE_PARAM_MA_BW_D,                                                                        
    PID_MEASURE_PARAM_MA_BW_E,                                                                        
    PID_MEASURE_PARAM_MA_CH_S,                                                                        
    PID_MEASURE_PARAM_MA_CH_D,                                                                        
    PID_MEASURE_PARAM_MA_CH_E,                                                                        
    PID_MEASURE_PARAM_MA_DT_R,                                                                        
    PID_MEASURE_PARAM_MA_BW_S_R,                                                                      
    PID_MEASURE_PARAM_MA_BW_D_R,                                                                      
    PID_MEASURE_PARAM_MA_BW_E_R,                                                                      
    PID_MEASURE_PARAM_MA_CH_S_R,                                                                      
    PID_MEASURE_PARAM_MA_CH_D_R,                                                                      
    PID_MEASURE_PARAM_MA_CH_E_R,                                                                      
    //--------------------------------------------                                              
    // �뿪 SKIP                                                      
    PID_MEASURE_PARAM_MA_BW_SKIP_S,                                                                 
    PID_MEASURE_PARAM_MA_BW_SKIP_D,                                                                 
    PID_MEASURE_PARAM_MA_BW_SKIP_E,                                                                 
                                                                                                
    PID_MEASURE_START_MA,                                                                       
    PID_MEASURE_STOP_MA,                                                                        
    PID_MEASURE_PARAM_MA,                                                                       
    PID_MEASURE_PARAM_MA_R,                                                                     
    PID_MEASURE_RESULT_MA_RX1,                                                                  
    PID_MEASURE_RESULT_MA_RX2,                                                                  
    PID_MEASURE_RESULT_MA_RX3,                                                                  
    PID_MEASURE_RESULT_MA_RX4,                                                                  
    //--------------------------------------------                                              
    PID_MEASURE_HOLD_MA,                                                                        
    PID_MEASURE_PLAY_MA,                                                                        
    PID_MEASURE_REC_STRAT_MA,                                                                   
    PID_MEASURE_REC_STOP_MA,                                                                    
    PID_MEASURE_SCAN_COMPLETE_MA,                                                               
    PID_MEASURE_SCAN_1_COMPLETE_MA,                                                             
    //--------------------------------------------		                                        

	PID_RX1_ERROR,                                                                         
	PID_RX1_INIT,                                                                          
	PID_RX2_ERROR,                                                                         
	PID_RX2_INIT,                                                                          
	PID_RX3_ERROR,                                                                         
	PID_RX3_INIT,                                                                          
	PID_RX4_ERROR,                                                                         
	PID_RX4_INIT,                                                                          
    PID_RX5_ERROR,                                                                         
    PID_RX5_INIT,                                                                          
                                                                                           
	PID_DEMOD_INIT,                                                                        
                                                                                           
	PID_RESERVED_01,                                                                       
	PID_RESERVED_02,                                                                       
	PID_RESERVED_03,                                                                       
	PID_RESERVED_04,                                                                       
	PID_RESERVED_05,                                                                       
	PID_RESERVED_06,                                                                       
	PID_RESERVED_07,                                                                       
	PID_RESERVED_08,                                                                       
	PID_RESERVED_09,                                                                       
	PID_RESERVED_10,                                                                       
                                                                                           
    //--------------------------------------------		                                   
    // Message Event Tag                                                                   
    //--------------------------------------------	                                       
	MID_RX1_CONNECT,                                                                       
	MID_RX2_CONNECT,                                                                       
	MID_RX3_CONNECT,                                                                       
	MID_RX4_CONNECT,                                                                       
    MID_RX5_CONNECT,                                                                       
	                                                                                       
	MID_RX1_DISCONNECT,                                                                    
	MID_RX2_DISCONNECT,                                                                    
	MID_RX3_DISCONNECT,                                                                    
	MID_RX4_DISCONNECT,                                                                    
    MID_RX5_DISCONNECT,                                                                    
                                                                                           
                                                                                           
	//	- DEM (����������ġ ���� ���μ���)                                                 
	MID_DEM_CONNECT,                                                                       
    	                                                                                   
	MID_RX_PARAMETERS_SET,              // ���ļ�,�뿪��,��������,����ġ ���� ����         
	MID_RX1_PARAMETERS_SET,             // ���ű�#1 ���ļ�,�뿪��,��������,����ġ ���� ����
	MID_RX2_PARAMETERS_SET,             // ���ű�#2 ���ļ�,�뿪��,��������,����ġ ���� ����
	MID_RX3_PARAMETERS_SET,             // ���ű�#3 ���ļ�,�뿪��,��������,����ġ ���� ����
	MID_RX4_PARAMETERS_SET,             // ���ű�#4 ���ļ�,�뿪��,��������,����ġ ���� ����
	MID_RX5_PARAMETERS_SET,             // ���ű�#5 ���ļ�,�뿪��,��������,����ġ ���� ����
                                                                                           
	MID_RX1_CURRENT_FREQ_REQ,           // ���ű�#1 ���� ���ļ�[AFC] ��û                  
	MID_RX2_CURRENT_FREQ_REQ,           // ���ű�#2 ���� ���ļ�[AFC] ��û                  
	MID_RX3_CURRENT_FREQ_REQ,           // ���ű�#3 ���� ���ļ�[AFC] ��û                  
	MID_RX4_CURRENT_FREQ_REQ,           // ���ű�#4 ���� ���ļ�[AFC] ��û                  
	MID_RX5_CURRENT_FREQ_REQ,           // ���ű�#5 ���� ���ļ�[AFC] ��û                  
                                                                                           
	MID_RX_REQUEST_LEVEL,               // ���ű� ��ȣ���� ��û                            
	MID_RX_REQUEST_LEVEL_MA,            // ��밨�� ��������: ��ȣ����                     
	MID_RX_REQUEST_LEVEL_SP,            // SPURIOUS : OnReceiveMessage �̺�Ʈ��
	MID_RX_REQUEST_LEVEL_SH,            // HARMONIC : OnReceiveMessage �̺�Ʈ��
	MID_RX_REQUEST_LEVEL_ME,            // ME(10MHz����) : OnReceiveMessage �̺�Ʈ��
	MID_RX_REQUEST_LEVEL_CAL,           // CALIBRATION : REQUEST LEVEL                     
	                                                                                       
	MID_RX_FSCAN_PARAM_SET,             // �뿪��ĵ �Ķ���� ����                          
	MID_RX_FSCAN_START,                                                                    
	MID_RX_FSCAN_STOP,                                                                     
    MID_RX_FSCAN_CONTINUE,              // ��밨�� FSCAN �����                           
                                                                                           
	MID_RX_MSCAN_PARAM_SET_STRAT,       // ä�ν�ĵ �Ķ���� ����/��ĵ����                 
	MID_RX_MSCAN_STOP,                                                                     
	MID_RX_CHANNEL_CLEAR,                                                                  
                                                                                           
	MID_RX_SWEEP_FREQ_SET,              // SWEEP�� ���ļ��� �����ϰ� ��ĵ�ϴ� ���         
	MID_RX_ATT_GAIN_SET,                // ATTENUATION / GAIN SETTING                      
                                                                                           
    MID_SYS_MON_CAL_SG_OFF,             // ������ �ý��ۺ��� ��ȣ�߻��� OFF 
    MID_SYS_DF_REQ,                     // [����ǰ�� ���� --> ��Ž����] ����ǰ�������� ���ÿ� ��Ž���� ��û 
    MID_SYS_DF_RES,                     // [��Ž���� --> ����ǰ�� ����] ���� ����� ��Ž ��� ����           
	//--------------------------------------------	
	
    //--------------------------------------------                              
    // ����Ž�� ����                                                            
    //--------------------------------------------
    PID_MON_CONFIRM_DF,                 // ����Ž�� ���� ����Ȯ��               
    PID_MON_CONNECT_DF,                 // ����Ž�� ���� ����                   
    PID_MON_DISCONNECT_DF,              // ����Ž�� ���� ���� ����              
    PID_MON_INIT_DF,                    // ����Ž�� ���� �ʱ�ȭ                 

	PID_MEASURE_PARAM_DF_ENV,           // ��Ž ȯ������     [���ʿ�]           
                                                                                
    PID_MEASURE_PARAM_DF_DT,                                                    
    PID_MEASURE_PARAM_DF_BW_S,                                                  
    PID_MEASURE_PARAM_DF_BW_D,                                                  
    PID_MEASURE_PARAM_DF_BW_E,                                                  
    PID_MEASURE_PARAM_DF_CH_S,                                                  
    PID_MEASURE_PARAM_DF_CH_D,                                                  
    PID_MEASURE_PARAM_DF_CH_E,                                                  
    PID_MEASURE_PARAM_ME_DF,	                                                
                                                                                
    PID_MEASURE_PARAM_DF_DT_R,                                                  
    PID_MEASURE_PARAM_DF_BW_S_R,
    PID_MEASURE_PARAM_DF_BW_D_R,                                                
    PID_MEASURE_PARAM_DF_BW_E_R,
    PID_MEASURE_PARAM_DF_CH_S_R,
    PID_MEASURE_PARAM_DF_CH_D_R,                                                
    PID_MEASURE_PARAM_DF_CH_E_R, 
                                                                    	        
    PID_MEASURE_PARAM_ME_DT_R,                                                  
                                                                                
    PID_MEASURE_START_DF,                                                       
    PID_MEASURE_STOP_DF,                                                        
    PID_MEASURE_COMPLETE_DF,                                                    
    PID_MEASURE_FFT_DF_INIT_ENV,                                                
    PID_MEASURE_FFT_DF_CURRENT,                                                 
    PID_MEASURE_FFT_CL_CURRENT,                                                 
    PID_MEASURE_RESULT_DF,                                                      
    PID_MEASURE_CAL_FULL,                                                       
    PID_MEASURE_CAL_FULL_R,                                                     
    PID_MEASURE_CAL_COMPLETE_FULL,                                              
    PID_MEASURE_CAL_TABLE,                                                      
    PID_MEASURE_CAL_TABLE_R,                                                    
    PID_MEASURE_CAL_COMPLETE_TABLE,                                             
    PID_MEASURE_START_RC,                                                       
    PID_MEASURE_STOP_RC,                                                        
    PID_MEASURE_COMPLETE_RC,                                                    
    PID_MEASURE_RESULT_RC,                                                      
    PID_MEASURE_SAVE_CAL_DATA,     
	
	PID_MEASURE_MR_DF_Q,				// ����ǰ������ ���ϸ�� ��Ž ��û
	PID_MEASURE_MR_DF_R,				// ����ǰ������ ���ϸ�� ��Ž ���
	PID_MEASURE_MR_DF_DENY,				// ����ǰ������ ���ϸ�� ��Ž �Ұ�
                                                                                
    PID_MEASURE_RESULT_DF_GPS,                                                  
    PID_FFT_RECEIVE,                    // [OP]	                                
    PID_SPEED_TEST,                     // ��Ʈ�� �ӵ� üũ [OP]                
    PID_MESSAGE_DATA,                   // [OP]                                 
                                                                                
    PID_SIGNAL_SIMULATE_START,                                                  
    PID_SIGNAL_SIMULATE_STOP,                                                   
    PID_SIGNAL_SIMULATE_RESULT,                                                 
    PID_SIGNAL_SIMULATE_SET,                                                    
    PID_SIGNAL_SIMULATE_RETURN,                                                 
                                                                                
    PID_EVALUATE_RX_SIGNAL_START,                                               
    PID_EVALUATE_RX_SIGNAL_STOP,                                                
    PID_EVALUATE_RX_SIGNAL_RESULT,                                              
	PID_EVALUATE_RX_SIGNAL_COMPLETE,                                            
                                                                                
    PID_CHANNEL_MISMATCH_START,                                                 
    PID_CHANNEL_MISMATCH_STOP,                                                  
    PID_CHANNEL_MISMATCH_RESULT_Q,                                              
    PID_CHANNEL_MISMATCH_RESULT_R,                                              
    PID_CHANNEL_MISMATCH_COMPLETE,                                              
    PID_CHANNEL_MISMATCH_SAVE,   
	
	//------------------------------------------
	// RF ��� ���� ����
	//------------------------------------------
	PID_DEVICE_STATUS_RFPATH_Q,			// RF ������� ��û
	PID_DEVICE_STATUS_RFPATH_R,			// RF ������� ���
  
	//------------------------------------------
	// RF ��� ���� ����
	//------------------------------------------
	PID_MEASURE_RFPATH_CAL_Q,			// RF ��κ��� ��û
	PID_MEASURE_RFPATH_CAL_R,			// RF ��κ��� ���

	//------------------------------------------
	// SG ��ü����
	//------------------------------------------
	PID_DEVICE_STATUS_SG_Q,				// ��ȣ�߻��� �ڱ����� ��û             
	PID_DEVICE_STATUS_SG_R,				// ��ȣ�߻��� �ڱ����� ����             
	PID_DEVICE_DETAIL_STATUS_SG_Q,		// ��ȣ�߻��� ������ ��û             
	PID_DEVICE_DETAIL_STATUS_SG_R,		// ��ȣ�߻��� ������ ����             

	//------------------------------------------
	// ��纸��		/// HAPPYIM [2010/6/11] ��Ž(�ؽ������) ����
	//------------------------------------------
	PID_MEASURE_PARAM_RADIATION_CAL_Q,                                                                    
	PID_MEASURE_PARAM_RADIATION_CAL_R,
	PID_MEASURE_PARAM_CHANNEL_LOSS_Q,
	PID_MEASURE_PARAM_CHANNEL_LOSS_R,

	//--------------------------------------------                              
    // �������� �� ������ġ ���� ����                                           
    //--------------------------------------------                              
    PID_VOICE_FILE_SRV_CONNECT       = 1000,                                    
    PID_VOICE_FILE_SRV_CONNECT_R,                                               
    PID_VOIPGW_CONNECT,                                                         
    PID_VOIPGW_CONNECT_R,                                                       
    PID_VOICE_FILE_SRV_DISCONNECT,                                              
    PID_VOIPGW_DISCONNECT,                                                      
    PID_VOICE_FILE_SRV_INIT_PORT_IP,                                            
    PID_VOIPGW_INIT_PORT_IP,                                                    
    PID_DEMOD_REC_STRAT,                                                        
    PID_DEMOD_REC_STOP,                                                         
    PID_DEMOD_STREAM_STRAT,                                                     
    PID_DEMOD_STREAM_STOP,                                                      
    PID_DEMOD_STREAM_TRANCELATE,                                                
    PID_DEMOD_REC_COMPLETE_MR,                                                  
    PID_DEMOD_REC_COMPLETE_MI,                                                  

    //--------------------------------------------                              
    // �������	(�̵����� <--> ��������)                                        
    //--------------------------------------------     

	PID_CONN_MRMS_CCS_CONNECT         = 2049,   // CCS, RMS ���� ����/����

    //--------------------------------------------
	// �ϰ�����(Batch Measurement Request) CCS -> MRMS
	//--------------------------------------------
	PID_CONN_CCS_MRMS_BATCH_MRDC_REQ  = 2100,   // ����ǰ������ ��û                                
	PID_CONN_CCS_MRMS_BATCH_MRAT_REQ,			// ����ǰ��-��ȣ�� ���� ��û                        

    PID_CONN_CCS_MRMS_BATCH_MIBW_REQ,           // �ҹ�����Ž�� ��û                                
    PID_CONN_CCS_MRMS_BATCH_MOBW_REQ,           // ������ �뿪 ��û                                 
	PID_CONN_CCS_MRMS_BATCH_MEDT_REQ,           // ���Ľ���Ʈ������ ��û                            
    PID_CONN_CCS_MRMS_BATCH_MPHC_REQ,           // ��ǻ���--������ ��û                          
    PID_CONN_CCS_MRMS_BATCH_MPSP_REQ,           // ��ǻ���--�ҿ��� ��û                          
	PID_CONN_CCS_MRMS_BATCH_STOP_REQ,			// �ϰ����� ���� ��û                               
    //--------------------------------------------
	// �ϰ�����(Batch Measurement Response) MRMS -> CCS
	//--------------------------------------------
	PID_CONN_MRMS_CCS_BATCH_END,				// �ϰ����� �Ϸ� ����                               
	PID_CONN_MRMS_CCS_BATCH_ERR,                // �ϰ����� ���� ����                               
    //--------------------------------------------
	// �󼼿�� ��Ȳ & ����Ʈ�� MRMS -> CCS ����
	//--------------------------------------------
	PID_CONN_MRMS_CCS_OPRTDSPCT_MRDC  = 2150,	// ����ǰ������ �󼼿�� ��Ȳ ���� ����Ʈ��         
	PID_CONN_MRMS_CCS_OPRTDSPCT_MRCH,			// ����ǰ������ �󼼿�� ��Ȳ ���� ����Ʈ��         
 	PID_CONN_MRMS_CCS_OPRTDSPCT_MIBW,           // �ҹ�����Ž�� �󼼿�� ��Ȳ ���� ����Ʈ��         
	PID_CONN_MRMS_CCS_OPRTDSPCT_MICH,           // �ҹ�����Ž�� �󼼿�� ��Ȳ ���� ����Ʈ��         
	PID_CONN_MRMS_CCS_OPRTDSPCT_MOBW,           // ���������� �󼼿�� ��Ȳ ���� ����Ʈ��           
	PID_CONN_MRMS_CCS_OPRTDSPCT_MOCH,           // ���������� �󼼿�� ��Ȳ ���� ����Ʈ��           
	PID_CONN_MRMS_CCS_OPRTDSPCT_MEDT,			// ��ӽ���Ʈ�� �󼼿�� ��Ȳ ���� ����Ʈ��         
	PID_CONN_MRMS_CCS_OPRTDSPCT_MPHC,           // ��ǻ�������-������ �󼼿�� ��Ȳ ���� ����Ʈ��
	PID_CONN_MRMS_CCS_OPRTDSPCT_MPSP,			// ��ǻ�������-�ҿ��� �󼼿�� ��Ȳ ���� ����Ʈ��
	//--------------------------------------------
	// ���Ȳ ������� ����ȯ���� MRMS -> CCS ����
	//--------------------------------------------
	PID_CONN_MRMS_CCS_MEASENV_MRDC,				// ����ǰ������ ���� ȯ��       
	PID_CONN_MRMS_CCS_MEASENV_MRCH,				// ����ǰ������ ä�� ȯ��       
	PID_CONN_MRMS_CCS_MEASENV_MIBW,             // �ҹ�����Ž�� �뿪 ȯ��       
	PID_CONN_MRMS_CCS_MEASENV_MICH,             // �ҹ�����Ž�� ä�� ȯ��       
	PID_CONN_MRMS_CCS_MEASENV_MOBW,             // ���������� �뿪 ȯ��         
	PID_CONN_MRMS_CCS_MEASENV_MOCH,             // ���������� ä�� ȯ��         
	PID_CONN_MRMS_CCS_MEASENV_MEDT,				// ���Ľ���Ʈ�� ȯ��            
	PID_CONN_MRMS_CCS_MEASENV_MPHC,             // ��ǻ�������-������ ȯ��   
	PID_CONN_MRMS_CCS_MEASENV_MPSP,				// ��ǻ�������-�ҿ��� ȯ��   
	//--------------------------------------------
	// ���Ȳ�󼼿� ��������� MRMS ->CCS �� ����
	//--------------------------------------------
	PID_CONN_MRMS_CCS_MEASRET_MRDC,				// ����ǰ��-���� ���� ���      
	PID_CONN_MRMS_CCS_MEASRET_MRCH,				// ����ǰ��-ä�� ���� ���      
	PID_CONN_MRMS_CCS_MEASRET_MRAT,				// ����ǰ��-��ȣ�� ���� ���    
	PID_CONN_MRMS_CCS_MEASRET_MIBW,             // �ҹ�����Ž�� ���            
	PID_CONN_MRMS_CCS_MEASRET_MICH,             // �ҹ�����Ž�� ���            
	PID_CONN_MRMS_CCS_MEASRET_MOBW,             // �������뿪 ���              
	PID_CONN_MRMS_CCS_MEASRET_MOCH,             // �������뿪 ���              
	PID_CONN_MRMS_CCS_MEASRET_MEDT,             // ���Ľ���Ʈ�� ���            
	PID_CONN_MRMS_CCS_MEASRET_MPHC,             // ��ǻ���-������ ���       
	PID_CONN_MRMS_CCS_MEASRET_MPSP,             // ��ǻ���-�ҿ��� ���       
	//------------------------------
	PID_CONN_CCS_MRMS_OPRTD_REQ       = 2200,   // �󼼿�� ��Ȳ���� ��û        

    //--------------------------------------------                                              
    // ������, ������Ž ���� ����                                                               
    //--------------------------------------------                                              
    PID_CONN_FDF_TO_RMS_AZI_REQ       = 3020,   // * RMS �ִ���Ź��������� ��û                
    PID_CONN_RMS_TO_FDF_AZI_RESULT,             // * RMS �ִ���Ź��������� ���                
    PID_CONN_FDF_TO_RMS_AZI_STOP,               // * RMS �ִ���Ź��������� ����                
    PID_CONN_RMS_TO_FDF_AZI_ERR,                // * RMS �ִ���Ź��������� ����                
                                               	//   ���������� �������� ��� ��ûó��          
                                             	//   �Ұ����� ������Ž���� �뺸                 
    PID_CONN_RMS_TO_FDF_STATUS_REQ    = 3040,   // * FDF �������� ��û				            
    PID_CONN_FDF_TO_RMS_STATUS_RES,             // * FDF �������� Ȯ��				            
    PID_CONN_RMS_TO_FDF_DF_REQ        = 3050,	// * FDF ��Žó�� ��û				            
    PID_CONN_RMS_TO_FDF_DF_STOP,                // * FDF ��Žó�� ����				            
    PID_CONN_FDF_TO_RMS_DF_RESULT     = 3060,	// * FDF ��Žó�� ���				            
    PID_CONN_RMS_TO_FDF_REQ_REPORT    = 3070,   // * FDF ó����� ����                          
                                                //   [Success/Error/Why?...]	                
    //--------------------------------------------                                              
    // ������, �̵���Ž �������	                                                            
    //--------------------------------------------                                              
    PID_CONN_MDF_LOGIN_GET	          = 4000,	// �̵���Ž ����� �α���                       
                                                // (CDMA ������ �뺸, DB Update / Insert)       
    PID_CONN_MDF_LOGOUT_GET,  		    		// �̵���Ž ����� �α׾ƿ�: MDF -> MBS         
    PID_CONN_CCS_TO_MDF_MSG_GET	      = 4010,	// ������������ �޽��� ����: CCS -> MBS         
    PID_CONN_CCS_TO_MDF_MSG_PUT,	    		// ������������ �޽��� ����: MBS -> MDF         
    PID_CONN_MDF_TO_CCS_MSG_GET,	    		// �̵���Ž���� �޽��� ����: MDF -> MBS         
    PID_CONN_MDF_TO_CCS_MSG_PUT,    			// �̵���Ž���� �޽��� ����: MBS -> CCS         
    PID_CONN_RMS_TO_MDF_MSG_GET	      = 4110,	// �������뿡�� �޽��� ����: RMS -> MBS         
    PID_CONN_RMS_TO_MDF_MSG_PUT,	   			// �������뿡�� �޽��� ����: MBS -> MDF         
    PID_CONN_MDF_TO_RMS_MSG_GET,	   			// �̵���Ž���� �޽��� ����: MDF -> MBS         
    PID_CONN_MDF_TO_RMS_MSG_PUT,    			// �̵���Ž���� �޽��� ����: MBS -> RMS         
    PID_CONN_MDF_TO_MDF_MSG_GET	      = 4410,	// �̵���Ž���� �޽��� ����: MDF -> MBS         
    PID_CONN_MDF_TO_MDF_MSG_PUT,				// �̵���Ž���� �޽��� ����: MBS -> MDF         
    PID_CONN_CCS_TO_MDF_DF_REQ_GET	  = 4050,	// ������������ ��Ž��û: CCS -> MBS            
    PID_CONN_CCS_TO_MDF_DF_REQ_PUT,		    	// ������������ ��Ž��û: MBS -> MDF            
    PID_CONN_MDF_TO_CCS_DF_RESULT_GET = 4060,	// ���������� ��Ž��� ����: MDF -> MBS         
    PID_CONN_MDF_TO_CCS_DF_RESULT_PUT,			// ���������� ��Ž��� ����: MBS -> CCS         
    PID_CONN_RMS_TO_MDF_DF_REQ_GET	  = 4150,	// �������뿡�� ��Ž��û: RMS -> MBS            
    PID_CONN_RMS_TO_MDF_DF_REQ_PUT,				// �������뿡�� ��Ž��û: MBS -> MDF            
    PID_CONN_RMS_TO_MDF_DF_RESULT_GET = 4160,	// ������������ ��Ž��� ����: MDF -> MBS       
    PID_CONN_RMS_TO_MDF_DF_RESULT_PUT,			// ������������ ��Ž��� ����: MBS -> RMS       
    PID_CONN_MDF_TO_MDF_DF_RESULT_GET = 4460,	// Ÿ �̵���Ž���� ��Ž��� ����: MDF -> MBS    
    PID_CONN_MDF_TO_MDF_DF_RESULT_PUT,			// Ÿ �̵���Ž���� ��Ž��� ����: MBS -> MDF    
    PID_CONN_CCS_TO_MDF_MOVE_REQ_GET  = 4030,	// ������������ �̵� ��û [����]: CCS -> MBS    
    PID_CONN_CCS_TO_MDF_MOVE_REQ_PUT,			// ������������ �̵� ��û [�뺸]: MBS -> MDF    
    PID_CONN_RMS_TO_MDF_MOVE_REQ_GET  = 4130,	// �������뿡�� �̵� ��û [����]: RMS -> MBS    
    PID_CONN_RMS_TO_MDF_MOVE_REQ_PUT,			// ������������ �̵� ��û [�뺸]: MBS -> MDF    
    PID_CONN_MDF_LICENSE_REQ_GET      = 4560,	// �̵���Ž �㰡DB��ȸ ��û : MDF -> MBS        
    PID_CONN_MDF_LICENSE_RESULT_PUT,			// �̵���Ž �㰡DB��ȸ ��� : MBS -> MDF        
    PID_CONN_MDF_MBS_CONTINUE,					// MDF�� MBS �� ��������                        
    PID_CONN_CCS_ALL_ID_SEND,					// ������������ �ڽ��� ID ���� ����             
                                                                                                
    PID_FILE_DELETE_VOIPGW_S          = 9001,   // ���ϸ�                                       
    PID_FILE_DELETE_VOIPGW_R,                   // ����ü                                       
    PID_FILE_DELETE_STATUS_VOIPGW_Q,            // ���ϸ�                                       
    PID_FILE_DELETE_STATUS_VOIPGW_R,            // ����ü                                       
    PID_EXIT_VOIPGW_S,
	
	PID_SET_SG_ON					  = 10001,	// ��ȣ�߻���: Output On
	PID_SET_SG_OFF,								// ��ȣ�߻���: Output Off
	PID_SET_SG_FREQ,							// ��ȣ�߻���: Freq Set (Hz)
	PID_SET_SG_LEVEL,							// ��ȣ�߻���: Level Set ( dBm)
	
	PID_BIG_TRANSFER					= 20001,
	
	PID_MEASURE_SIGNAL_LEVEL			= 20002,	// ��ȣ���� ��ȸ
	PID_MEASURE_SIGNAL_LEVEL_R,
	
	PID_MEASURE_IQ_DF_RESULT_R			= 20004,	// ������ ��Ž ��� ��û�� ���� PID
	
	PID_MEASURE_CAL_DATA				= 20005,
	PID_MEASURE_CAL_DATA_R,

	emRmsUDP_ServicePortInit_S_TAG		= 29821,
	emRmsUDP_ServicePortInit_R_TAG,

	PID_MAKING_MI_CHANNEL				= 30000,

	emRmsPacketFFT_InitEnv_R_TAG		= 30200,	// OP���� Env ���� ����

	PID_PACKET_END           
	
} enumPacketTag;

#endif // DEF_ENUM_PACKET_TAG


#endif	// END #ifndef __DEF_SOCKET__
