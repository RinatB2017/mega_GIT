#include <cudaarithm.hpp>
#include <opencv2/highgui.hpp>

extern "C"
struct TensorWrapper minCuda(
        struct cutorchInfo info, struct TensorWrapper src1, struct TensorWrapper src2, struct TensorWrapper dst)
{
    GpuMatT dstMat = dst.toGpuMatT();
    cuda::GpuMat & mat = dstMat.mat;
    cuda::min(src1.toGpuMat(), src2.toGpuMat(), dstMat, prepareStream(info));
    return TensorWrapper(dstMat, info.state);
}

extern "C"
struct TensorWrapper maxCuda(
        struct cutorchInfo info, struct TensorWrapper src1, struct TensorWrapper src2, struct TensorWrapper dst)
{
    if (dst.isNull()) {
        cuda::GpuMat result;
        cuda::max(src1.toGpuMat(), src2.toGpuMat(), result, prepareStream(info));
        return TensorWrapper(result, info.state);
    } else {
        cuda::max(src1.toGpuMat(), src2.toGpuMat(), dst.toGpuMat(), prepareStream(info));
        return dst;
    }
}

extern "C"
struct TensorPlusDouble thresholdCuda(
        struct cutorchInfo info, struct TensorWrapper src,
        struct TensorWrapper dst, double thresh, double maxval, int type)
{
    TensorPlusDouble retval;

    if (dst.isNull()) {
        cuda::GpuMat result;
        retval.val = cuda::threshold(src.toGpuMat(), result, thresh, maxval, type, prepareStream(info));
        new (&retval.tensor) TensorWrapper(result, info.state);
    } else {
        retval.val = cuda::threshold(src.toGpuMat(), dst.toGpuMat(), thresh, maxval, type, prepareStream(info));
        retval.tensor = dst;
    }
    return retval;
}

extern "C"
struct TensorWrapper magnitudeCuda(
        struct cutorchInfo info, struct TensorWrapper xy, struct TensorWrapper magnitude)
{
    if (magnitude.isNull()) {
        cuda::GpuMat result;
        cuda::magnitude(xy.toGpuMat(), result, prepareStream(info));
        return TensorWrapper(result, info.state);
    } else {
        cuda::magnitude(xy.toGpuMat(), magnitude.toGpuMat(), prepareStream(info));
        return magnitude;
    }
}

extern "C"
struct TensorWrapper magnitudeSqrCuda(
        struct cutorchInfo info, struct TensorWrapper xy, struct TensorWrapper magnitude)
{
    if (magnitude.isNull()) {
        cuda::GpuMat result;
        cuda::magnitudeSqr(xy.toGpuMat(), result, prepareStream(info));
        return TensorWrapper(result, info.state);
    } else {
        cuda::magnitudeSqr(xy.toGpuMat(), magnitude.toGpuMat(), prepareStream(info));
        return magnitude;
    }
}

extern "C"
struct TensorWrapper magnitude2Cuda(
        struct cutorchInfo info, struct TensorWrapper x, struct TensorWrapper y, struct TensorWrapper magnitude)
{
    if (magnitude.isNull()) {
        cuda::GpuMat result;
        cuda::magnitude(x.toGpuMat(), y.toGpuMat(), result, prepareStream(info));
        return TensorWrapper(result, info.state);
    } else {
        cuda::magnitude(x.toGpuMat(), y.toGpuMat(), magnitude.toGpuMat(), prepareStream(info));
        return magnitude;
    }
}

extern "C"
struct TensorWrapper magnitudeSqr2Cuda(
        struct cutorchInfo info, struct TensorWrapper x, struct TensorWrapper y, struct TensorWrapper magnitudeSqr)
{
    if (magnitudeSqr.isNull()) {
        cuda::GpuMat result;
        cuda::magnitudeSqr(x.toGpuMat(), y.toGpuMat(), result, prepareStream(info));
        return TensorWrapper(result, info.state);
    } else {
        cuda::magnitudeSqr(x.toGpuMat(), y.toGpuMat(), magnitudeSqr.toGpuMat(), prepareStream(info));
        return magnitudeSqr;
    }
}

extern "C"
struct TensorWrapper phaseCuda(
        struct cutorchInfo info, struct TensorWrapper x, struct TensorWrapper y,
        struct TensorWrapper angle, bool angleInDegrees)
{
    if (angle.isNull()) {
        cuda::GpuMat result;
        cuda::phase(x.toGpuMat(), y.toGpuMat(), result, angleInDegrees, prepareStream(info));
        return TensorWrapper(result, info.state);
    } else {
        cuda::phase(x.toGpuMat(), y.toGpuMat(), angle.toGpuMat(), angleInDegrees, prepareStream(info));
        return angle;
    }
}

extern "C"
struct TensorArray cartToPolarCuda(
        struct cutorchInfo info, struct TensorWrapper x, struct TensorWrapper y,
        struct TensorWrapper magnitude, struct TensorWrapper angle, bool angleInDegrees)
{
    std::vector<cuda::GpuMat> result(2);
    result[0] = magnitude.toGpuMat();
    result[1] = angle.toGpuMat();

    cuda::cartToPolar(x.toGpuMat(), y.toGpuMat(), result[0], result[1], angleInDegrees, prepareStream(info));

    return TensorArray(result, info.state);
}

extern "C"
struct TensorArray polarToCartCuda(
        struct cutorchInfo info, struct TensorWrapper magnitude, struct TensorWrapper angle,
        struct TensorWrapper x, struct TensorWrapper y, bool angleInDegrees)
{
    std::vector<cuda::GpuMat> result;
    result[0] = x.toGpuMat();
    result[1] = y.toGpuMat();

    cuda::polarToCart(magnitude.toGpuMat(), angle.toGpuMat(), result[0], result[1], angleInDegrees, prepareStream(info));

    return TensorArray(result, info.state);
}

extern "C"
struct LookUpTablePtr LookUpTable_ctorCuda(
        struct cutorchInfo info, struct TensorWrapper lut)
{
    return rescueObjectFromPtr(cuda::createLookUpTable(lut.toGpuMat()));
}

extern "C"
struct TensorWrapper LookUpTable_transformCuda(
        struct cutorchInfo info, struct LookUpTablePtr ptr,
        struct TensorWrapper src, struct TensorWrapper dst)
{
    if (dst.isNull()) {
        cuda::GpuMat result;
        ptr->transform(src.toGpuMat(), result);
        return TensorWrapper(result, info.state);
    } else {
        ptr->transform(src.toGpuMat(), dst.toGpuMat(), prepareStream(info));
        return dst;
    }
}

extern "C"
struct TensorWrapper rectStdDevCuda(
        struct cutorchInfo info, struct TensorWrapper src, struct TensorWrapper sqr,
        struct TensorWrapper dst, struct RectWrapper rect)
{
    if (dst.isNull()) {
        cv::Mat result;
        cuda::rectStdDev(src.toGpuMat(), sqr.toGpuMat(), result, rect, prepareStream(info));
        return TensorWrapper(result);
    } else {
        cuda::rectStdDev(src.toGpuMat(), sqr.toGpuMat(), dst.toGpuMat(), rect, prepareStream(info));
        return dst;
    }
}

extern "C"
struct TensorWrapper normalizeCuda(
        struct cutorchInfo info, struct TensorWrapper src, struct TensorWrapper dst,
        double alpha, double beta, int norm_type, int dtype, struct TensorWrapper mask)
{
    if (dst.isNull()) {
        cv::Mat result;
        cuda::normalize(src.toGpuMat(), result, alpha, beta, norm_type,
                        dtype, TO_MAT_OR_NOARRAY(mask), prepareStream(info));
        return TensorWrapper(result);
    } else {
        cuda::normalize(src.toGpuMat(), dst.toGpuMat(), alpha, beta, norm_type,
                        dtype, TO_MAT_OR_NOARRAY(mask), prepareStream(info));
        return dst;
    }
}

extern "C"
struct TensorWrapper integralCuda(
        struct cutorchInfo info, struct TensorWrapper src, struct TensorWrapper sum)
{
    if (sum.isNull()) {
        cv::Mat result;
        cuda::integral(src.toGpuMat(), result, prepareStream(info));
        return TensorWrapper(result);
    } else {
        cuda::integral(src.toGpuMat(), sum.toGpuMat(), prepareStream(info));
        return sum;
    }
}

extern "C"
struct TensorWrapper sqrIntegralCuda(
        struct cutorchInfo info, struct TensorWrapper src, struct TensorWrapper sum)
{
    if (sum.isNull()) {
        cv::Mat result;
        cuda::sqrIntegral(src.toGpuMat(), result, prepareStream(info));
        return TensorWrapper(result);
    } else {
        cuda::sqrIntegral(src.toGpuMat(), sum.toGpuMat(), prepareStream(info));
        return sum;
    }
}

extern "C"
struct TensorWrapper mulSpectrumsCuda(
        struct cutorchInfo info, struct TensorWrapper src1, struct TensorWrapper src2,
        struct TensorWrapper dst, int flags, bool conjB)
{
    if (dst.isNull()) {
        cuda::GpuMat result;
        cuda::mulSpectrums(src1.toGpuMat(), src2.toGpuMat(), result, flags, conjB, prepareStream(info));
        return TensorWrapper(result, info.state);
    } else {
        cuda::mulSpectrums(src1.toGpuMat(), src2.toGpuMat(), dst.toGpuMat(), flags, conjB, prepareStream(info));
        return dst;
    }
}

extern "C"
struct TensorWrapper mulAndScaleSpectrumsCuda(
        struct cutorchInfo info, struct TensorWrapper src1, struct TensorWrapper src2,
        struct TensorWrapper dst, int flags, float scale, bool conjB)
{
    if (dst.isNull()) {
        cuda::GpuMat result;
        cuda::mulAndScaleSpectrums(src1.toGpuMat(), src2.toGpuMat(), result, flags, scale, conjB, prepareStream(info));
        return TensorWrapper(result, info.state);
    } else {
        cuda::mulAndScaleSpectrums(src1.toGpuMat(), src2.toGpuMat(), dst.toGpuMat(), flags, scale, conjB, prepareStream(info));
        return dst;
    }
}

extern "C"
struct TensorWrapper dftCuda(
        struct cutorchInfo info, struct TensorWrapper src,
        struct TensorWrapper dst, struct SizeWrapper dft_size, int flags)
{
    if (dst.isNull()) {
        cv::Mat result;
        cuda::dft(src.toGpuMat(), result, dft_size, flags, prepareStream(info));
        return TensorWrapper(result);
    } else {
        cuda::dft(src.toGpuMat(), dst.toGpuMat(), dft_size, flags, prepareStream(info));
        return dst;
    }
}

extern "C"
struct ConvolutionPtr Convolution_ctorCuda(
        struct cutorchInfo info, struct SizeWrapper user_block_size)
{
    return rescueObjectFromPtr(cuda::createConvolution(user_block_size));
}

extern "C"
struct TensorWrapper Convolution_convolveCuda(
        struct cutorchInfo info, struct ConvolutionPtr ptr, struct TensorWrapper image,
        struct TensorWrapper templ, struct TensorWrapper result, bool ccor)
{
    if (result.isNull()) {
        cuda::GpuMat resultMat;
        ptr->convolve(image.toGpuMat(), templ.toGpuMat(), resultMat, ccor, prepareStream(info));
        return TensorWrapper(resultMat, info.state);
    } else {
        ptr->convolve(image.toGpuMat(), templ.toGpuMat(), result.toGpuMat(), ccor);
        return result;
    }
}
