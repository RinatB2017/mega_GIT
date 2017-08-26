local cv = require 'cv._env'
require 'cutorch'

-- TODO: remove this after gathering all CUDA packages in a single submodule
cv.cuda = cv.cuda or require 'cv._env_cuda'

local ffi = require 'ffi'

ffi.cdef[[
struct EncoderParams {
    int P_Interval;      //!< NVVE_P_INTERVAL,
    int IDR_Period;      //!< NVVE_IDR_PERIOD,
    int DynamicGOP;      //!< NVVE_DYNAMIC_GOP,
    int RCType;          //!< NVVE_RC_TYPE,
    int AvgBitrate;      //!< NVVE_AVG_BITRATE,
    int PeakBitrate;     //!< NVVE_PEAK_BITRATE,
    int QP_Level_Intra;  //!< NVVE_QP_LEVEL_INTRA,
    int QP_Level_InterP; //!< NVVE_QP_LEVEL_INTER_P,
    int QP_Level_InterB; //!< NVVE_QP_LEVEL_INTER_B,
    int DeblockMode;     //!< NVVE_DEBLOCK_MODE,
    int ProfileLevel;    //!< NVVE_PROFILE_LEVEL,
    int ForceIntra;      //!< NVVE_FORCE_INTRA,
    int ForceIDR;        //!< NVVE_FORCE_IDR,
    int ClearStat;       //!< NVVE_CLEAR_STAT,
    int DIMode;          //!< NVVE_SET_DEINTERLACE,
    int Presets;         //!< NVVE_PRESETS,
    int DisableCabac;    //!< NVVE_DISABLE_CABAC,
    int NaluFramingType; //!< NVVE_CONFIGURE_NALU_FRAMING_TYPE
    int DisableSPSPPS;   //!< NVVE_DISABLE_SPS_PPS
};

struct FormatInfo {
    int codec;
    int chromaFormat;
    int width;
    int height;
};

struct EncoderParams EncoderParams_ctor_defaultCuda();

struct EncoderParams EncoderParams_ctorCuda(const char *configFile);

void EncoderParams_saveCuda(struct EncoderParams params, const char *configFile);

struct PtrWrapper VideoWriter_ctorCuda();

void VideoWriter_dtorCuda(struct PtrWrapper ptr);

void VideoWriter_writeCuda(struct PtrWrapper ptr, struct TensorWrapper frame, bool lastFrame);

struct EncoderParams VideoWriter_getEncoderParamsCuda(struct PtrWrapper ptr);

struct PtrWrapper VideoReader_ctorCuda(const char *filename);

void VideoReader_dtorCuda(struct PtrWrapper ptr);

struct TensorWrapper VideoReader_nextFrameCuda(
        struct cutorchInfo info, struct PtrWrapper ptr, struct TensorWrapper frame);

struct FormatInfo VideoReader_formatCuda(struct PtrWrapper ptr);
]]

local C = ffi.load(cv.libPath('cudacodec'))

require 'cv.Classes'
local Classes = ffi.load(cv.libPath('Classes'))

-- TODO test this on Windows
do
    local EncoderParams = torch.class('cuda.EncoderParams', cv.cuda)

    local paramNames = {
        P_Interval = true,
        IDR_Period = true,
        DynamicGOP = true,
        RCType = true,
        AvgBitrate = true,
        PeakBitrate = true,
        QP_Level_Intra = true,
        QP_Level_InterP = true,
        QP_Level_InterB = true,
        DeblockMode = true,
        ProfileLevel = true,
        ForceIntra = true,
        ForceIDR = true,
        ClearStat = true,
        DIMode = true,
        Presets = true,
        DisableCabac = true,
        NaluFramingType = true,
        DisableSPSPPS = true
    }

    function EncoderParams:__init(t)
        local argRules = {
            {"configFile", default = nil}
        }
        local configFile = cv.argcheck(t, argRules)

        if configFile then
            self.object = C.EncoderParams_ctorCuda(configFile)
        else
            self.object = C.EncoderParams_ctor_defaultCuda()
        end
    end

    function EncoderParams:__index__(key)
        if paramNames[key] ~= nil then
            return rawget(self, object)[key]
        else
            return rawget(self, key)
        end
    end

    function EncoderParams:load(t)
        local argRules = {
            {"configFile", required = true}
        }
        local configFile = cv.argcheck(t, argRules)

        self.object = C.EncoderParams_ctorCuda(configFile)
    end

    function EncoderParams:save(t)
        local argRules = {
            {"configFile", required = true}
        }
        local configFile = cv.argcheck(t, argRules)

        C.EncoderParams_saveCuda(self.object, configFile)
    end
end

do
    local VideoWriter = torch.class('cuda.VideoWriter', cv.cuda)

    function VideoWriter:__init(t)
        local argRules = {
            {"fileName", required = true},
            {"frameSize", required = true, operator = cv.Size},
            {"fps", required = true},
            {"params", default = cv.cuda.EncoderParams{}},
            {"format", default = cv.cuda.SF_BGR}
        }
        local fileName, frameSize, fps, params, format = cv.argcheck(t, argRules)

        assert(torch.type(params) == 'cuda.EncoderParams')

        self.ptr = ffi.gc(C.VideoWriter_ctorCuda(
            fileName, frameSize, fps, params, format), C.VideoWriter_dtorCuda)
    end

    function VideoWriter:write(t)
        local argRules = {
            {"frame", required = true},
            {"lastFrame", default = false}
        }
        local frame, lastFrame = cv.argcheck(t, argRules)

        C.VideoWriter_writeCuda(self.ptr, cv.wrap_tensor(frame), lastFrame)
    end

    function VideoWriter:getEncoderParams()
        local retval = cv.cuda.EncoderParams{}
        retval.object = C.VideoWriter_getEncoderParamsCuda(self.ptr)
        return retval
    end
end

do
    local VideoReader = torch.class('cuda.VideoReader', cv.cuda)

    function VideoReader:__init(t)
        local argRules = {
            {"filename", required = true}
        }
        local filename = cv.argcheck(t, argRules)

        self.ptr = ffi.gc(C.VideoReader_ctorCuda(filename), C.VideoReader_dtorCuda)
    end

    function VideoReader:nextFrame(t)
        local argRules = {
            {"frame", default = nil}
        }
        local frame = cv.argcheck(t, argRules)

        return cv.unwrap_tensors(C.VideoReader_nextFrameCuda(cv.cuda._info(),
            self.ptr, cv.wrap_tensor(frame)))
    end

    function VideoReader:format()
        return C.VideoReader_formatCuda(self.ptr)
    end
end

return cv.cuda
