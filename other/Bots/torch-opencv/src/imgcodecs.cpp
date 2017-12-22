#include <imgcodecs.hpp>

extern "C"
struct TensorWrapper imread(const char *filename, int flags)
{
    return TensorWrapper(cv::imread(filename, flags));
}

extern "C"
struct TensorArrayPlusBool imreadmulti(const char *filename, int flags)
{
    TensorArrayPlusBool retval;
    std::vector<cv::Mat> mats;
    retval.val = cv::imreadmulti(filename, mats, flags);
    new (&retval.tensors) TensorArray(mats);
    return retval;
}

extern "C"
bool imwrite(const char *filename, struct TensorWrapper img, struct TensorWrapper params)
{
    if (params.isNull())
        return cv::imwrite(filename, img.toMat());
    else
        return cv::imwrite(filename, img.toMat(), params.toMat());
}

extern "C"
struct TensorWrapper imdecode(struct TensorWrapper buf, int flags)
{
    return TensorWrapper(cv::imdecode(buf.toMat(), flags));
}

extern "C"
struct TensorWrapper imencode(
        const char *ext, struct TensorWrapper img, struct TensorWrapper params)
{
    std::vector<unsigned char> retval;
    cv::imencode(ext, img.toMat(), retval, params.toMat());
    return TensorWrapper(cv::Mat(retval, true));
}
