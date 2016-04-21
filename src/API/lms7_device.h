/* 
 * File:   lms7_device.h
 * Author: ignas
 *
 * Created on March 9, 2016, 12:54 PM
 */

#ifndef LMS7_DEVICE_H
#define	LMS7_DEVICE_H
#include "LMS7002M.h"
#include "lms7_api.h"
#include <mutex>
#include "LMS64CProtocol.h" //TODO remove when reset usb is abstracted

typedef struct
{
    bool enabled;
    bool half_duplex;
    float_type bandwidth;
    float_type lpf_bw;
    float_type nco_pho;
    
} lms_channel_info;

class LMS7_Device : public lime::LMS7002M
{
public:
    LMS7_Device();
    virtual ~LMS7_Device();
    virtual int EnableTX(size_t ch,bool);
    virtual int EnableRX(size_t ch,bool);
    virtual int ConfigureSamplePositions();
    virtual int Init();
    virtual int SetReferenceClock(const float_type refCLK_MHz);
    virtual size_t GetNumChannels(const bool tx) const;
    virtual int SetRate(float_type f_MHz, int oversample);
    virtual int SetRate(bool tx, float_type f_MHz, size_t oversample = 0);
    virtual float_type GetRate(bool tx,size_t chan,float_type *rf_rate_Hz = NULL);
    virtual lms_range_t GetRxRateRange(const size_t chan = 0)const;
    virtual lms_range_t GetTxRateRange(const size_t chan = 0)const;
    virtual std::vector<std::string> GetPathNames(bool dir_tx, size_t chan) const;
    virtual int SetPath(bool tx,size_t chan, size_t path);
    virtual size_t GetPath(bool tx, size_t chan);
    virtual int SetRxFrequency(size_t chan, float_type f_Hz);
    virtual int SetTxFrequency(size_t chan, float_type f_Hz);
    virtual lms_range_t GetFrequencyRange(bool tx) const;
    virtual lms_range_t GetRxPathBand(size_t path, size_t chan) const;
    virtual lms_range_t GetTxPathBand(size_t path, size_t chan) const;
    virtual int SetBandwidth(bool tx,size_t chan, float_type bandwidth);
    virtual float_type GetBandwidth(bool tx,size_t chan);
    virtual lms_range_t GetBandwidthRange(bool tx) const;
    virtual int SetLPF(bool tx, size_t chan, bool f, bool en, float_type bandwidth=-1);
    virtual float_type GetLPFBW(bool tx,size_t chan, bool filt);
    virtual lms_range_t GetLPFRange(bool tx,size_t chan,bool f);  
    virtual int SetGFIRCoef(bool tx, size_t chan, lms_gfir_t filt, const float_type* coef,size_t count);
    virtual int GetGFIRCoef(bool tx, size_t chan, lms_gfir_t filt, float_type* coef);
    virtual int SetGFIR(bool tx, size_t chan, lms_gfir_t filt, bool enabled);
    virtual int SetDCOffset(bool dir_tx, size_t chan, int16_t dc_i, int16_t dc_q);
    virtual int GetDCOffset(bool dir_tx, size_t chan, int16_t *dc_i, int16_t *dc_q);
    virtual lms_range_t GetDCOffsetRange(bool dir_tx) const ;
    virtual int SetIQGain(bool dir_tx, size_t chan, unsigned gain_i, unsigned gain_q);
    virtual int GetIQGain(bool dir_tx, size_t chan, unsigned *gain_i, unsigned *gain_q);
    virtual lms_range_t GetIQGainRange(bool dir_tx) const;
    virtual int SetIQPhase(bool dir_tx, size_t chan, int16_t phase);
    virtual int16_t GetIQPhase(bool dir_tx, size_t chan);
    virtual lms_range_t GetIQPhaseRange(bool dir_tx) const;
    virtual int CalibrateCh(bool tx,size_t chan,float_type bandwidth);
    virtual int SetNormalizedGain(bool dir_tx, size_t chan,float_type gain);
    virtual float_type GetNormalizedGain(bool dir_tx, size_t chan);
    virtual int SetTestSignal(bool dir_tx, size_t chan,lms_testsig_t sig,int16_t dc_i =0, int16_t dc_q = 0);
    virtual int GetTestSignal(bool dir_tx, size_t chan);
    virtual int SetNCOFreq(bool tx,size_t ch, const float_type *freq, float_type pho);
    virtual int SetNCO(bool tx,size_t ch,size_t ind,bool down);
    virtual int GetNCOFreq(bool tx,size_t ch, float_type * freq,float_type *pho);
    virtual int SetNCOPhase(bool tx,size_t ch, const float_type *phase, float_type fcw);
    virtual int GetNCOPhase(bool tx,size_t ch, float_type * phase,float_type *fcw);
    virtual size_t GetNCO(bool tx,size_t ch);
    virtual int SetStreamingMode(uint32_t flags);
    virtual int ConfigureRxStream(size_t numBuffers, size_t bufSize,size_t fifo);
    virtual int ConfigureTxStream(size_t numBuffers, size_t bufSize,size_t fifo);
    virtual int RecvStream(void **samples,size_t sample_count, lms_stream_metadata *meta, unsigned timeout_ms);
    virtual int SendStream(const void **samples,size_t sample_count, lms_stream_metadata *meta, unsigned timeout_ms);
    virtual int ProgramFPGA(const char* data, size_t len, lms_storage_t mode);
    virtual int ProgramFPGA(std::string name, lms_storage_t mode);
    virtual int ProgramFW(const char* data, size_t len, lms_storage_t mode);
    virtual int ProgramFW(std::string name, lms_storage_t mode);
    virtual int ProgramMCU(const char* data= nullptr, size_t len=0, lms_storage_t mode=LMS_STORAGE_RAM);
    virtual int DACWrite(uint16_t val);
    virtual int DACRead();
    
private:
    lms_channel_info* tx_channels;
    lms_channel_info* rx_channels;
    static const double LMS_CGEN_MAX;
    int RecvStreamFloat(float **data, int16_t* buffer, size_t numSamples, uint64_t* ts, int &index, uint64_t* rx_meta, unsigned timeout_ms);
    int SendStreamFloat(const float **data, int16_t* buffer, size_t numSamples, uint64_t ts, int &index, uint64_t tx_meta, unsigned timeout_ms);
    int RecvStreamInt16(int16_t **data, int16_t* buffer, size_t numSamples, uint64_t* ts, int &index, uint64_t* rx_meta, unsigned timeout_ms);
    int SendStreamInt16(const int16_t **data, int16_t* buffer, size_t numSamples, uint64_t ts, int &index, uint64_t tx_meta, unsigned timeout_ms);
    int ConfigureRXLPF(bool enabled,int ch,float_type bandwidth);
    int ConfigureTXLPF(bool enabled,int ch,float_type bandwidth);
    int ConfigureGFIR(bool enabled,bool tx, float_type bandwidth,size_t ch);
    int _read(int16_t *data, uint64_t *timestamp, uint64_t *metadata,unsigned timeout);
    int _write(int16_t *data, uint64_t timestamp, uint64_t metadata, unsigned timeout);
    int StartRx(int packetsToBatch=4, int numTransfers=32);
    int StartTx(int packetsToBatch=4, int numTransfers=32);
    int StopRx();
    int StopTx();
    int Start();
    int Stop();
    
    void ResetUSBFIFO();
    
    bool rx_running;
    bool tx_running;

    std::mutex rx_lock;
    std::mutex tx_lock;
    int channelsCount;
    enum { MODE_AUTO,
           MODE_SISO,
           MODE_MIMO
    }forced_mode;
    enum { 
        FMT_INT16,
        FMT_FLOAT,
    }sample_fmt;
  
    int rx_packetsToBatch;
   
    int rx_buffersCount; // must be power of 2   
    int *rx_handles;
    char *rx_buffers;
    
    int tx_packetsToBatch;
    int tx_buffersCount; // must be power of 2   
    int *tx_handles;
    char *tx_buffers;
    bool *tx_bufferUsed;
    
};

#endif	/* LMS7_DEVICE_H */
