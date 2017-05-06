/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2014 Texas Instruments Inc.
 */

#ifndef GRABBER_H
#define GRABBER_H

#include <deque>
#include <string>
#include <CameraSystem.h>
#include <DepthCamera.h>
#include <Common.h>
#include <unistd.h>
#include <termio.h>
#include <functional>
#include <stdint.h>

using namespace std;
using namespace Voxel;

class Grabber
{
public:

    typedef enum {
        FRAMEFLAG_RAW_UNPROCESSED = (1<<DepthCamera::FRAME_RAW_FRAME_UNPROCESSED),
        FRAMEFLAG_RAW_PROCESSED = (1<<DepthCamera::FRAME_RAW_FRAME_PROCESSED),
        FRAMEFLAG_DEPTH_FRAME = (1<<DepthCamera::FRAME_DEPTH_FRAME),
        FRAMEFLAG_XYZI_POINT_CLOUD_FRAME = (1<<DepthCamera::FRAME_XYZI_POINT_CLOUD_FRAME),
        FRAMEFLAG_ALL = (FRAMEFLAG_RAW_UNPROCESSED
                        | FRAMEFLAG_RAW_PROCESSED
                        | FRAMEFLAG_DEPTH_FRAME
                        | FRAMEFLAG_XYZI_POINT_CLOUD_FRAME)
    } FrameFlag;

protected:
    DepthCameraPtr _depthCamera;
    
    CameraSystem &_sys;

    Mutex _mtx;
    
    deque<DepthFrame* > _qDepthFrame;
    deque<XYZIPointCloudFrame* > _qXYZIFrame;
    deque<ToFRawFrameTemplate<uint16_t,uint8_t> * > _qProcessedFrame;
    deque<ToFRawFrameTemplate<uint16_t,uint8_t> * > _qUnprocessedFrame;
    
    FrameFlag _frameFlag;

    uint32_t _frameCount;  
    
    bool _updateDone;
    
    int _rows, _cols;
    
    std::function<void(Grabber *g)> _update;
    
    void _callback(DepthCamera &depthCamera, const Frame &frame, 
            DepthCamera::FrameType type);

    void _applyFilter();

public:
    Grabber(DepthCameraPtr depthCamera, FrameFlag flag, CameraSystem &sys);

    virtual float getFramesPerSecond() const
    {
        FrameRate r;
        if(!_depthCamera->getFrameRate(r))
      	    return 0;
    	else
      	    return r.getFrameRate();
  	}

    virtual DepthCameraPtr getDepthCamera() { return _depthCamera; }

    virtual std::string getName() const { return _depthCamera->id(); }

    virtual bool isRunning() const { return _depthCamera->isRunning(); }

    virtual void start() { _depthCamera->start(); }

    virtual void stop() { _depthCamera->stop(); _depthCamera->wait(); }

    virtual uint32_t getFrameCount();

    virtual FrameFlag getFrameFlag() { return _frameFlag; }
    
    virtual XYZIPointCloudFrame * getXYZIFrame();
    
    virtual DepthFrame * getDepthFrame();

    virtual ToFRawFrameTemplate<uint16_t,uint8_t> * getRawFrameProcessed();

    virtual ToFRawFrameTemplate<uint16_t,uint8_t> * getRawFrameUnprocessed();

    virtual void setFrameRate(float frameRate);
    
    virtual void getFrameSize(int &rows, int &cols) { rows=_rows; cols=_cols; }

    bool getSerialNumber(std::string &str);

    virtual const Map<int, Voxel::String> &getProfiles();

    virtual bool setProfile(Voxel::String name);
    
    virtual bool setRegister(Voxel::String name, uint value)
                    { return _depthCamera->set(name, value); }
                        
    virtual void registerUpdate(std::function<void(Grabber *g)> update);
    
    virtual void updateExit() { if (isRunning()) stop(); _updateDone = true; }
    
    virtual void run();

    virtual ~Grabber()
    {
        if(isRunning())
            stop();
    }

};


#endif // GRABBER_H
