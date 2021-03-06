#ifndef VSB_SEMESTRAL_PROJECT_HASHING_H
#define VSB_SEMESTRAL_PROJECT_HASHING_H

#include <opencv2/opencv.hpp>
#include "../core/hash_table.h"
#include "../core/template_group.h"
#include "../core/window.h"

/**
 * class Hasher
 *
 * Class used to train templates and sliding windows, to prefilter
 * number of templates needed to be template matched in other stages of
 * template matching.
 */
class Hasher {
private:
    int minVotesPerTemplate;
    cv::Size referencePointsGrid;
    unsigned int maxTripletDistance;
    unsigned int hashTableCount;
    unsigned int histogramBinCount;
    std::vector<cv::Range> histogramBinRanges;

    // Methods
    cv::Vec3d extractSurfaceNormal(const cv::Mat &src, const cv::Point c);
    cv::Vec2i extractRelativeDepths(const cv::Mat &src, const cv::Point c, const cv::Point p1, const cv::Point p2);

    int quantizeSurfaceNormals(cv::Vec3f normal);
    int quantizeDepths(float depth);

    void generateTriplets(std::vector<HashTable> &hashTables);
    void calculateDepthHistogramRanges(unsigned long histogramSum, unsigned long histogramValues[]);
    void calculateDepthBinRanges(const std::vector<TemplateGroup> &groups, std::vector<HashTable> &hashTables);
public:
    // Statics
    static const int IMG_16BIT_VALUE_MAX;
    static const int IMG_16BIT_VALUES_RANGE;

    // Constructors
    Hasher(int minVotesPerTemplate = 3, cv::Size referencePointsGrid = cv::Size(12, 12),
           unsigned int hashTableCount = 100, unsigned int histogramBinCount = 5, unsigned int maxTripletDistance = 3)
        : minVotesPerTemplate(minVotesPerTemplate), referencePointsGrid(referencePointsGrid),
          hashTableCount(hashTableCount), histogramBinCount(histogramBinCount), maxTripletDistance(maxTripletDistance) {}

    // Methods
    void initialize(const std::vector<TemplateGroup> &groups, std::vector<HashTable> &hashTables);
    void train(std::vector<TemplateGroup> &groups, std::vector<HashTable> &hashTables);
    void verifyTemplateCandidates(const cv::Mat &sceneDepth, std::vector<HashTable> &hashTables, std::vector<Window> &windows);

    // Getters
    const cv::Size getReferencePointsGrid();
    unsigned int getHashTableCount() const;
    const std::vector<cv::Range> &getHistogramBinRanges() const;
    unsigned int getHistogramBinCount() const;
    int getMinVotesPerTemplate() const;
    unsigned int getMaxTripletDistance() const;

    // Setters
    void setReferencePointsGrid(cv::Size referencePointsGrid);
    void setHashTableCount(unsigned int hashTableCount);
    void setHistogramBinRanges(const std::vector<cv::Range> &histogramBinRanges);
    void setHistogramBinCount(unsigned int histogramBinCount);
    void setMinVotesPerTemplate(int minVotesPerTemplate);
    void setMaxTripletDistance(unsigned int maxTripletDistance);
};

#endif //VSB_SEMESTRAL_PROJECT_HASHING_H
