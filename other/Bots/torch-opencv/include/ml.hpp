#include <Common.hpp>
#include <Classes.hpp>
#include <opencv2/ml.hpp>

namespace ml = cv::ml;

struct TensorWrapper randMVNormal(
        struct TensorWrapper mean, struct TensorWrapper cov, int nsamples, struct TensorWrapper samples);

struct TensorArray createConcentricSpheresTestSet(
        int nsamples, int nfeatures, int nclasses, struct TensorWrapper samples, struct TensorWrapper responses);

struct ParamGridPtr {
    void *ptr;

    inline ml::ParamGrid * operator->() { return static_cast<ml::ParamGrid *>(ptr); }
    inline ParamGridPtr(ml::ParamGrid *ptr) { this->ptr = ptr; }

    inline operator ml::ParamGrid & () { return *static_cast<ml::ParamGrid *>(ptr); }
};

struct TrainDataPtr {
    void *ptr;

    inline ml::TrainData * operator->() { return static_cast<ml::TrainData *>(ptr); }
    inline TrainDataPtr(ml::TrainData *ptr) { this->ptr = ptr; }
    inline operator ml::TrainData *() { return static_cast<ml::TrainData *>(ptr); }
};

struct StatModelPtr {
    void *ptr;

    inline ml::StatModel * operator->() { return static_cast<ml::StatModel *>(ptr); }
    inline StatModelPtr(ml::StatModel *ptr) { this->ptr = ptr; }
};

extern "C"
struct ParamGridPtr ParamGrid_ctor(double _minVal, double _maxVal, double _logStep);

extern "C"
struct ParamGridPtr ParamGrid_ctor_default();

extern "C"
void ParamGrid_dtor(struct ParamGridPtr ptr);

extern "C"
struct TrainDataPtr TrainData_ctor(
        struct TensorWrapper samples, int layout, struct TensorWrapper responses,
        struct TensorWrapper varIdx, struct TensorWrapper sampleIdx,
        struct TensorWrapper sampleWeights, struct TensorWrapper varType);

extern "C"
void TrainData_dtor(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getSubVector(struct TensorWrapper vec, struct TensorWrapper idx);

extern "C"
int TrainData_getLayout(struct TrainDataPtr ptr);

extern "C"
int TrainData_getNTrainSamples(struct TrainDataPtr ptr);

extern "C"
int TrainData_getNTestSamples(struct TrainDataPtr ptr);

extern "C"
int TrainData_getNSamples(struct TrainDataPtr ptr);

extern "C"
int TrainData_getNVars(struct TrainDataPtr ptr);

extern "C"
int TrainData_getNAllVars(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getSamples(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getMissing(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getTrainResponses(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getTrainNormCatResponses(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getTestResponses(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getTestNormCatResponses(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getResponses(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getNormCatResponses(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getSampleWeights(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getTrainSampleWeights(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getTestSampleWeights(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getVarIdx(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getVarType(struct TrainDataPtr ptr);

extern "C"
int TrainData_getResponseType(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getTrainSampleIdx(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getTestSampleIdx(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getDefaultSubstValues(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getClassLabels(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getCatOfs(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getCatMap(struct TrainDataPtr ptr);

extern "C"
void TrainData_shuffleTrainTest(struct TrainDataPtr ptr);

extern "C"
struct TensorWrapper TrainData_getSample(
        struct TrainDataPtr ptr, struct TensorWrapper varIdx, int sidx);

extern "C"
struct TensorWrapper TrainData_getTrainSamples(
        struct TrainDataPtr ptr, int layout, bool compressSamples, bool compressVars);

extern "C"
struct TensorWrapper TrainData_getValues(
        struct TrainDataPtr ptr, int vi, struct TensorWrapper sidx);

extern "C"
struct TensorWrapper TrainData_getNormCatValues(
        struct TrainDataPtr ptr, int vi, struct TensorWrapper sidx);

extern "C"
void TrainData_setTrainTestSplit(struct TrainDataPtr ptr, int count, bool shuffle);

extern "C"
void TrainData_setTrainTestSplitRatio(struct TrainDataPtr ptr, double ratio, bool shuffle);

extern "C"
int StatModel_getVarCount(struct StatModelPtr ptr);

extern "C"
bool StatModel_empty(struct StatModelPtr ptr);

extern "C"
bool StatModel_isTrained(struct StatModelPtr ptr);

extern "C"
bool StatModel_isClassifier(struct StatModelPtr ptr);

extern "C"
bool StatModel_train(struct StatModelPtr ptr, struct TrainDataPtr trainData, int flags);

extern "C"
bool StatModel_train_Mat(
        struct StatModelPtr ptr, struct TensorWrapper samples, int layout, struct TensorWrapper responses);

extern "C"
struct TensorPlusFloat StatModel_calcError(
        struct StatModelPtr ptr, struct TrainDataPtr data, bool test, struct TensorWrapper resp);

extern "C"
float StatModel_predict(
        struct StatModelPtr ptr, struct TensorWrapper samples, struct TensorWrapper results, int flags);

struct NormalBayesClassifierPtr {
    void *ptr;

    inline ml::NormalBayesClassifier * operator->() { return static_cast<ml::NormalBayesClassifier *>(ptr); }
    inline NormalBayesClassifierPtr(ml::NormalBayesClassifier *ptr) { this->ptr = ptr; }
};

extern "C"
struct NormalBayesClassifierPtr NormalBayesClassifier_load(const char *filename, const char *objname);

extern "C"
struct NormalBayesClassifierPtr NormalBayesClassifier_ctor();

extern "C"
struct TensorArrayPlusFloat NormalBayesClassifier_predictProb(
        struct NormalBayesClassifierPtr ptr, struct TensorWrapper inputs,
        struct TensorWrapper outputs, struct TensorWrapper outputProbs, int flags);

struct KNearestPtr {
    void *ptr;

    inline ml::KNearest * operator->() { return static_cast<ml::KNearest *>(ptr); }
    inline KNearestPtr(ml::KNearest *ptr) { this->ptr = ptr; }
};

extern "C"
struct KNearestPtr KNearest_load(const char *filename, const char *objname);

extern "C"
struct KNearestPtr KNearest_ctor();

extern "C"
void KNearest_setDefaultK(struct KNearestPtr ptr, int val);

extern "C"
int KNearest_getDefaultK(struct KNearestPtr ptr);

extern "C"
void KNearest_setIsClassifier(struct KNearestPtr ptr, bool val);

extern "C"
bool KNearest_getIsClassifier(struct KNearestPtr ptr);

extern "C"
void KNearest_setEmax(struct KNearestPtr ptr, int val);

extern "C"
int KNearest_getEmax(struct KNearestPtr ptr);

extern "C"
void KNearest_setAlgorithmType(struct KNearestPtr ptr, int val);

extern "C"
int KNearest_getAlgorithmType(struct KNearestPtr ptr);

extern "C"
float KNearest_findNearest(
        struct KNearestPtr ptr, struct TensorWrapper samples, int k,
        struct TensorWrapper results, struct TensorWrapper neighborResponses,
        struct TensorWrapper dist);

struct SVMPtr {
    void *ptr;

    inline ml::SVM * operator->() { return static_cast<ml::SVM *>(ptr); }
    inline SVMPtr(ml::SVM *ptr) { this->ptr = ptr; }
};

extern "C"
struct SVMPtr SVM_load(const char *filename, const char *objname);

extern "C"
struct SVMPtr SVM_ctor();

extern "C"
void SVM_setType(struct SVMPtr ptr, int val);

extern "C"
int SVM_getType(struct SVMPtr ptr);

extern "C"
void SVM_setGamma(struct SVMPtr ptr, double val);

extern "C"
double SVM_getGamma(struct SVMPtr ptr);

extern "C"
void SVM_setCoef0(struct SVMPtr ptr, double val);

extern "C"
double SVM_getCoef0(struct SVMPtr ptr);

extern "C"
void SVM_setDegree(struct SVMPtr ptr, double val);

extern "C"
double SVM_getDegree(struct SVMPtr ptr);

extern "C"
void SVM_setC(struct SVMPtr ptr, double val);

extern "C"
double SVM_getC(struct SVMPtr ptr);

extern "C"
void SVM_setNu(struct SVMPtr ptr, double val);

extern "C"
double SVM_getNu(struct SVMPtr ptr);

extern "C"
void SVM_setP(struct SVMPtr ptr, double val);

extern "C"
double SVM_getP(struct SVMPtr ptr);

extern "C"
void SVM_setClassWeights(struct SVMPtr ptr, struct TensorWrapper val);

extern "C"
struct TensorWrapper SVM_getClassWeights(struct SVMPtr ptr);

extern "C"
void SVM_setTermCriteria(struct SVMPtr ptr, struct TermCriteriaWrapper val);

extern "C"
struct TermCriteriaWrapper SVM_getTermCriteria(struct SVMPtr ptr);

extern "C"
int SVM_getKernelType(struct SVMPtr ptr);

extern "C"
void SVM_setKernel(struct SVMPtr ptr, int val);

//extern "C"
//void SVM_setCustomKernel(struct SVMPtr ptr, struct KernelPtr val);

extern "C"
bool SVM_trainAuto(
        struct SVMPtr ptr, struct TrainDataPtr data, int kFold, struct ParamGridPtr Cgrid,
        struct ParamGridPtr gammaGrid, struct ParamGridPtr pGrid, struct ParamGridPtr nuGrid,
        struct ParamGridPtr coeffGrid, struct ParamGridPtr degreeGrid, bool balanced);

extern "C"
struct TensorWrapper SVM_getSupportVectors(struct SVMPtr ptr);

extern "C"
struct TensorArrayPlusDouble SVM_getDecisionFunction(
        struct SVMPtr ptr, int i, struct TensorWrapper alpha, struct TensorWrapper svidx);

extern "C"
struct ParamGridPtr SVM_getDefaultGrid(int param_id);

struct EMPtr {
    void *ptr;

    inline ml::EM * operator->() { return static_cast<ml::EM *>(ptr); }
    inline EMPtr(ml::EM *ptr) { this->ptr = ptr; }
};

extern "C"
struct EMPtr EM_load(const char *filename, const char *objname);

extern "C"
struct EMPtr EM_ctor();

extern "C"
void EM_setClustersNumber(struct EMPtr ptr, int val);

extern "C"
int EM_getClustersNumber(struct EMPtr ptr);

extern "C"
void EM_setCovarianceMatrixType(struct EMPtr ptr, int val);

extern "C"
int EM_getCovarianceMatrixType(struct EMPtr ptr);

extern "C"
void EM_setTermCriteria(struct EMPtr ptr, struct TermCriteriaWrapper val);

extern "C"
struct TermCriteriaWrapper EM_getTermCriteria(struct EMPtr ptr);

extern "C"
struct TensorWrapper EM_getWeights(struct EMPtr ptr);

extern "C"
struct TensorWrapper EM_getMeans(struct EMPtr ptr);

extern "C"
struct TensorArray EM_getCovs(struct EMPtr ptr);

extern "C"
struct Vec2dWrapper EM_predict2(
        struct EMPtr ptr, struct TensorWrapper sample, struct TensorWrapper probs);

extern "C"
bool EM_trainEM(
        struct EMPtr ptr, struct TensorWrapper samples,
        struct TensorWrapper logLikelihoods,
        struct TensorWrapper labels, struct TensorWrapper probs);

extern "C"
bool EM_trainE(
        struct EMPtr ptr, struct TensorWrapper samples, struct TensorWrapper means0,
        struct TensorWrapper covs0, struct TensorWrapper weights0,
        struct TensorWrapper logLikelihoods, struct TensorWrapper labels,
        struct TensorWrapper probs);

extern "C"
bool EM_trainM(
        struct EMPtr ptr, struct TensorWrapper samples, struct TensorWrapper probs0,
        struct TensorWrapper logLikelihoods, struct TensorWrapper labels,
        struct TensorWrapper probs);

struct DTreesPtr {
    void *ptr;

    inline ml::DTrees * operator->() { return static_cast<ml::DTrees *>(ptr); }
    inline DTreesPtr(ml::DTrees *ptr) { this->ptr = ptr; }
};

struct ConstNodeArray {
    const ml::DTrees::Node *ptr;
    int size;
};

struct ConstSplitArray {
    const ml::DTrees::Split *ptr;
    int size;
};

extern "C"
struct DTreesPtr DTrees_ctor();

extern "C"
struct DTreesPtr DTrees_load(const char *filename, const char *objname);

extern "C"
void DTrees_setMaxCategories(struct DTreesPtr ptr, int val);

extern "C"
int DTrees_getMaxCategories(struct DTreesPtr ptr);

extern "C"
void DTrees_setMaxDepth(struct DTreesPtr ptr, int val);

extern "C"
int DTrees_getMaxDepth(struct DTreesPtr ptr);

extern "C"
void DTrees_setMinSampleCount(struct DTreesPtr ptr, int val);

extern "C"
int DTrees_getMinSampleCount(struct DTreesPtr ptr);

extern "C"
void DTrees_setCVFolds(struct DTreesPtr ptr, int val);

extern "C"
int DTrees_getCVFolds(struct DTreesPtr ptr);

extern "C"
void DTrees_setUseSurrogates(struct DTreesPtr ptr, bool val);

extern "C"
bool DTrees_getUseSurrogates(struct DTreesPtr ptr);

extern "C"
void DTrees_setUse1SERule(struct DTreesPtr ptr, bool val);

extern "C"
bool DTrees_getUse1SERule(struct DTreesPtr ptr);

extern "C"
void DTrees_setTruncatePrunedTree(struct DTreesPtr ptr, bool val);

extern "C"
bool DTrees_getTruncatePrunedTree(struct DTreesPtr ptr);

extern "C"
void DTrees_setRegressionAccuracy(struct DTreesPtr ptr, float val);

extern "C"
float DTrees_getRegressionAccuracy(struct DTreesPtr ptr);

extern "C"
void DTrees_setPriors(struct DTreesPtr ptr, struct TensorWrapper val);

extern "C"
struct TensorWrapper DTrees_getPriors(struct DTreesPtr ptr);

extern "C"
struct TensorWrapper DTrees_getRoots(struct DTreesPtr ptr);

extern "C"
struct ConstNodeArray DTrees_getNodes(struct DTreesPtr ptr);

extern "C"
struct ConstSplitArray DTrees_getSplits(struct DTreesPtr ptr);

extern "C"
struct TensorWrapper DTrees_getSubsets(struct DTreesPtr ptr);

struct RTreesPtr {
    void *ptr;

    inline ml::RTrees * operator->() { return static_cast<ml::RTrees *>(ptr); }
    inline RTreesPtr(ml::RTrees *ptr) { this->ptr = ptr; }
};

extern "C"
struct RTreesPtr RTrees_load(const char *filename, const char *objname);

extern "C"
struct RTreesPtr RTrees_ctor();

extern "C"
void RTrees_setCalculateVarImportance(struct RTreesPtr ptr, bool val);

extern "C"
bool RTrees_getCalculateVarImportance(struct RTreesPtr ptr);

extern "C"
void RTrees_setActiveVarCount(struct RTreesPtr ptr, int val);

extern "C"
int RTrees_getActiveVarCount(struct RTreesPtr ptr);

extern "C"
void RTrees_setTermCriteria(struct RTreesPtr ptr, struct TermCriteriaWrapper val);

struct BoostPtr {
    void *ptr;

    inline ml::Boost * operator->() { return static_cast<ml::Boost *>(ptr); }
    inline BoostPtr(ml::Boost *ptr) { this->ptr = ptr; }
};

extern "C"
struct BoostPtr Boost_load(const char *filename, const char *objname);

extern "C"
struct BoostPtr Boost_ctor();

extern "C"
void Boost_setBoostType(struct BoostPtr ptr, int val);

extern "C"
int Boost_getBoostType(struct BoostPtr ptr);

extern "C"
void Boost_setWeakCount(struct BoostPtr ptr, int val);

extern "C"
int Boost_getWeakCount(struct BoostPtr ptr);

extern "C"
void Boost_setWeightTrimRate(struct BoostPtr ptr, double val);

extern "C"
double Boost_getWeightTrimRate(struct BoostPtr ptr);

struct ANN_MLPPtr {
    void *ptr;

    inline ml::ANN_MLP * operator->() { return static_cast<ml::ANN_MLP *>(ptr); }
    inline ANN_MLPPtr(ml::ANN_MLP *ptr) { this->ptr = ptr; }
};

extern "C"
struct ANN_MLPPtr ANN_MLP_load(const char *filename, const char *objname);

extern "C"
struct ANN_MLPPtr ANN_MLP_ctor();

extern "C"
void ANN_MLP_setTrainMethod(struct ANN_MLPPtr ptr, int method, double param1, double param2);

extern "C"
int ANN_MLP_getTrainMethod(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setActivationFunction(struct ANN_MLPPtr ptr, int type, double param1, double param2);

extern "C"
void ANN_MLP_setLayerSizes(struct ANN_MLPPtr ptr, struct TensorWrapper val);

extern "C"
struct TensorWrapper ANN_MLP_getLayerSizes(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setTermCriteria(struct ANN_MLPPtr ptr, struct TermCriteriaWrapper val);

extern "C"
struct TermCriteriaWrapper ANN_MLP_getTermCriteria(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setBackpropWeightScale(struct ANN_MLPPtr ptr, double val);

extern "C"
double ANN_MLP_getBackpropWeightScale(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setBackpropMomentumScale(struct ANN_MLPPtr ptr, double val);

extern "C"
double ANN_MLP_getBackpropMomentumScale(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setRpropDW0(struct ANN_MLPPtr ptr, double val);

extern "C"
double ANN_MLP_getRpropDW0(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setRpropDWPlus(struct ANN_MLPPtr ptr, double val);

extern "C"
double ANN_MLP_getRpropDWPlus(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setRpropDWMinus(struct ANN_MLPPtr ptr, double val);

extern "C"
double ANN_MLP_getRpropDWMinus(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setRpropDWMin(struct ANN_MLPPtr ptr, double val);

extern "C"
double ANN_MLP_getRpropDWMin(struct ANN_MLPPtr ptr);

extern "C"
void ANN_MLP_setRpropDWMax(struct ANN_MLPPtr ptr, double val);

extern "C"
double ANN_MLP_getRpropDWMax(struct ANN_MLPPtr ptr);

extern "C"
struct TensorWrapper ANN_MLP_getWeights(struct ANN_MLPPtr ptr, int layerIdx);

struct LogisticRegressionPtr {
    void *ptr;

    inline ml::LogisticRegression * operator->() { return static_cast<ml::LogisticRegression *>(ptr); }
    inline LogisticRegressionPtr(ml::LogisticRegression *ptr) { this->ptr = ptr; }
};

extern "C"
struct LogisticRegressionPtr LogisticRegression_load(const char *filename, const char *objname);

extern "C"
struct LogisticRegressionPtr LogisticRegression_ctor();

extern "C"
void LogisticRegression_setLearningRate(struct LogisticRegressionPtr ptr, double val);

extern "C"
double LogisticRegression_getLearningRate(struct LogisticRegressionPtr ptr);

extern "C"
void LogisticRegression_setIterations(struct LogisticRegressionPtr ptr, int val);

extern "C"
int LogisticRegression_getIterations(struct LogisticRegressionPtr ptr);

extern "C"
void LogisticRegression_setRegularization(struct LogisticRegressionPtr ptr, int val);

extern "C"
int LogisticRegression_getRegularization(struct LogisticRegressionPtr ptr);

extern "C"
void LogisticRegression_setTrainMethod(struct LogisticRegressionPtr ptr, int val);

extern "C"
int LogisticRegression_getTrainMethod(struct LogisticRegressionPtr ptr);

extern "C"
void LogisticRegression_setMiniBatchSize(struct LogisticRegressionPtr ptr, int val);

extern "C"
int LogisticRegression_getMiniBatchSize(struct LogisticRegressionPtr ptr);

extern "C"
void LogisticRegression_setTermCriteria(struct LogisticRegressionPtr ptr, struct TermCriteriaWrapper val);

extern "C"
struct TermCriteriaWrapper LogisticRegression_getTermCriteria(struct LogisticRegressionPtr ptr);

extern "C"
struct TensorWrapper LogisticRegression_get_learnt_thetas(struct LogisticRegressionPtr ptr);
