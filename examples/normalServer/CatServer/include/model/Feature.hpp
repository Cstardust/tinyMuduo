#ifndef _FEATURE_H_
#define _FEATURE_H_

#include"public.h"

class Feature
{
public:
    Feature(int featureId = -1, const string &personality = "", const string &appearance = "")
        : featureId_(featureId), personality_(personality), appearance_(appearance)
    {
    }

    int getFeatureId() { return featureId_; }
    string getPersonality() { return personality_; }
    string getAppearance() { return appearance_; }
    void setFeatureId(int featureId) { featureId_ = featureId; }
    void setPersonality(const string &personality) { personality_ = personality; }
    void setAppearance(const string &appearance) { appearance_ = appearance; }

private:
    int featureId_;
    string personality_;
    string appearance_;
};


#endif