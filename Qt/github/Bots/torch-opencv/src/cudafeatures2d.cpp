#include <cudafeatures2d.hpp>

extern "C"
struct DescriptorMatcherPtr createBFMatcherCuda(int normType) {
    return rescueObjectFromPtr(cuda::DescriptorMatcher::createBFMatcher(normType));
}

extern "C"
bool DescriptorMatcher_isMaskSupportedCuda(struct DescriptorMatcherPtr ptr) {
    return ptr->isMaskSupported();
}

extern "C"
void DescriptorMatcher_addCuda(
        struct DescriptorMatcherPtr ptr, struct TensorArray descriptors) {
    ptr->add(descriptors.toGpuMatList());
}

extern "C"
struct TensorArray DescriptorMatcher_getTrainDescriptorsCuda(
        struct cutorchInfo info, struct DescriptorMatcherPtr ptr) {
    std::vector<cuda::GpuMat> retval = ptr->getTrainDescriptors();
    return TensorArray(retval, info.state);
}

extern "C"
void DescriptorMatcher_clearCuda(struct DescriptorMatcherPtr ptr) {
    ptr->clear();
}

extern "C"
bool DescriptorMatcher_emptyCuda(struct DescriptorMatcherPtr ptr) {
    return ptr->empty();
}

extern "C"
void DescriptorMatcher_trainCuda(struct DescriptorMatcherPtr ptr) {
    ptr->train();
}

extern "C"
struct TensorWrapper DescriptorMatcher_matchCuda(
        struct cutorchInfo info, struct DescriptorMatcherPtr ptr,
        struct TensorWrapper queryDescriptors, struct TensorWrapper trainDescriptors,
        struct TensorWrapper matches, struct TensorWrapper mask)
{
    cuda::GpuMat retval = matches.toGpuMat();
    ptr->matchAsync(
            queryDescriptors.toGpuMat(), trainDescriptors.toGpuMat(), retval,
            TO_GPUMAT_OR_NOARRAY(mask), prepareStream(info));
    return TensorWrapper(retval, info.state);
}

extern "C"
struct TensorWrapper DescriptorMatcher_match_masksCuda(
        struct cutorchInfo info, struct DescriptorMatcherPtr ptr,
        struct TensorWrapper queryDescriptors, struct TensorWrapper matches,
        struct TensorArray masks)
{
    cuda::GpuMat retval = matches.toGpuMat();
    ptr->matchAsync(
            queryDescriptors.toGpuMat(), retval,
            TO_GPUMAT_LIST_OR_NOARRAY(masks), prepareStream(info));
    return TensorWrapper(retval, info.state);
}

extern "C"
struct DMatchArray DescriptorMatcher_matchConvertCuda(
         struct DescriptorMatcherPtr ptr, struct TensorWrapper gpu_matches)
{
    std::vector<cv::DMatch> retval;
    ptr->matchConvert(gpu_matches.toGpuMat(), retval);
    return DMatchArray(retval);
}

extern "C"
struct TensorWrapper DescriptorMatcher_knnMatchCuda(
        struct cutorchInfo info, struct DescriptorMatcherPtr ptr,
        struct TensorWrapper queryDescriptors, struct TensorWrapper trainDescriptors,
        struct TensorWrapper matches, int k, struct TensorWrapper mask)
{
    cuda::GpuMat retval = matches.toGpuMat();
    ptr->knnMatchAsync(
            queryDescriptors.toGpuMat(), trainDescriptors.toGpuMat(), retval,
            k, TO_GPUMAT_OR_NOARRAY(mask), prepareStream(info));
    return TensorWrapper(retval, info.state);
}

extern "C"
struct TensorWrapper DescriptorMatcher_knnMatch_masksCuda(
        struct cutorchInfo info, struct DescriptorMatcherPtr ptr,
        struct TensorWrapper queryDescriptors, struct TensorWrapper trainDescriptors,
        struct TensorWrapper matches, int k, struct TensorArray masks)
{
    cuda::GpuMat retval = matches.toGpuMat();
    ptr->knnMatchAsync(
            queryDescriptors.toGpuMat(), trainDescriptors.toGpuMat(), retval,
            k, TO_GPUMAT_LIST_OR_NOARRAY(masks), prepareStream(info));
    return TensorWrapper(retval, info.state);
}

extern "C"
struct DMatchArrayOfArrays DescriptorMatcher_knnMatchConvertCuda(
        struct DescriptorMatcherPtr ptr,
        struct TensorWrapper gpu_matches, bool compactResult)
{
    std::vector<std::vector<cv::DMatch>> retval;
    ptr->knnMatchConvert(gpu_matches.toGpuMat(), retval, compactResult);
    return DMatchArrayOfArrays(retval);
}

extern "C"
struct TensorWrapper DescriptorMatcher_radiusMatchCuda(
        struct cutorchInfo info, struct DescriptorMatcherPtr ptr,
        struct TensorWrapper queryDescriptors, struct TensorWrapper trainDescriptors,
        struct TensorWrapper matches, float maxDistance, struct TensorWrapper mask)
{
    cuda::GpuMat retval = matches.toGpuMat();
    ptr->radiusMatchAsync(
            queryDescriptors.toGpuMat(), trainDescriptors.toGpuMat(),
            retval, maxDistance, TO_GPUMAT_OR_NOARRAY(mask), prepareStream(info));
    return TensorWrapper(retval, info.state);
}

extern "C"
struct TensorWrapper DescriptorMatcher_radiusMatch_masksCuda(
        struct cutorchInfo info, struct DescriptorMatcherPtr ptr,
        struct TensorWrapper queryDescriptors, struct TensorWrapper trainDescriptors,
        struct TensorWrapper matches, float maxDistance, struct TensorArray masks)
{
    cuda::GpuMat retval = matches.toGpuMat();
    ptr->radiusMatchAsync(
            queryDescriptors.toGpuMat(), trainDescriptors.toGpuMat(),
            retval, maxDistance, TO_GPUMAT_LIST_OR_NOARRAY(masks), prepareStream(info));
    return TensorWrapper(retval, info.state);
}

extern "C"
struct DMatchArrayOfArrays DescriptorMatcher_radiusMatchConvertCuda(
        struct DescriptorMatcherPtr ptr,
        struct TensorWrapper gpu_matches, bool compactResult)
{
    std::vector<std::vector<cv::DMatch>> retval;
    ptr->radiusMatchConvert(gpu_matches.toGpuMat(), retval, compactResult);
    return DMatchArrayOfArrays(retval);
}

extern "C"
void Feature2DAsync_dtorCuda(struct Feature2DAsyncPtr ptr)
{
    delete static_cast<cuda::Feature2DAsync *>(ptr.ptr);
}

extern "C"
struct TensorWrapper Feature2DAsync_detectAsyncCuda(
        struct cutorchInfo info, struct Feature2DAsyncPtr ptr, struct TensorWrapper image,
        struct TensorWrapper keypoints, struct TensorWrapper mask)
{
    cuda::GpuMat retval = keypoints.toGpuMat();
    ptr->detectAsync(image.toGpuMat(), retval, TO_GPUMAT_OR_NOARRAY(mask), prepareStream(info));
    return TensorWrapper(retval, info.state);
}

extern "C"
struct TensorArray Feature2DAsync_computeAsyncCuda(
        struct cutorchInfo info, struct Feature2DAsyncPtr ptr, struct TensorWrapper image,
        struct TensorWrapper keypoints, struct TensorWrapper descriptors)
{
    std::vector<cuda::GpuMat> retval(2);
    retval[0] = keypoints.toGpuMat();
    retval[1] = descriptors.toGpuMat();
    ptr->computeAsync(image.toGpuMat(), retval[0], retval[1], prepareStream(info));
    return TensorArray(retval, info.state);
}

extern "C"
struct TensorArray Feature2DAsync_detectAndComputeAsyncCuda(
        struct cutorchInfo info, struct Feature2DAsyncPtr ptr, struct TensorWrapper image,
        struct TensorWrapper mask, struct TensorWrapper keypoints,
        struct TensorWrapper descriptors, bool useProvidedKeypoints)
{
    std::vector<cuda::GpuMat> retval(2);
    retval[0] = keypoints.toGpuMat();
    retval[1] = descriptors.toGpuMat();
    ptr->detectAndComputeAsync(
            image.toGpuMat(), mask.toGpuMat(), retval[0], retval[1],
            useProvidedKeypoints, prepareStream(info));
    return TensorArray(retval, info.state);
}

extern "C"
struct KeyPointArray Feature2DAsync_convertCuda(
        struct Feature2DAsyncPtr ptr, struct TensorWrapper gpu_keypoints)
{
    std::vector<cv::KeyPoint> retval;
    ptr->convert(gpu_keypoints.toGpuMat(), retval);
    return KeyPointArray(retval);
}

extern "C"
struct FastFeatureDetectorPtr FastFeatureDetector_ctorCuda(
        int threshold, bool nonmaxSuppression, int type, int max_npoints)
{
    return rescueObjectFromPtr(
            cuda::FastFeatureDetector::create(threshold, nonmaxSuppression, type, max_npoints));
}

extern "C"
void FastFeatureDetector_setMaxNumPointsCuda(struct FastFeatureDetectorPtr ptr, int val)
{
    ptr->setMaxNumPoints(val);
}

extern "C"
int FastFeatureDetector_getMaxNumPointsCuda(struct FastFeatureDetectorPtr ptr)
{
    return ptr->getMaxNumPoints();
}

extern "C"
struct ORBPtr ORB_ctorCuda(
        int nfeatures, float scaleFactor, int nlevels, int edgeThreshold, int firstLevel,
        int WTA_K, int scoreType, int patchSize, int fastThreshold, bool blurForDescriptor)
{
    return rescueObjectFromPtr(
            cuda::ORB::create(nfeatures, scaleFactor, nlevels, edgeThreshold, firstLevel,
            WTA_K, scoreType, patchSize, fastThreshold, blurForDescriptor));
}

extern "C"
void ORB_setBlurForDescriptorCuda(struct ORBPtr ptr, bool val)
{
    ptr->setBlurForDescriptor(val);
}

extern "C"
bool ORB_getBlurForDescriptorCuda(struct ORBPtr ptr)
{
    return ptr->getBlurForDescriptor();
}
