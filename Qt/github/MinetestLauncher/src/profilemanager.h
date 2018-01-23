#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include "profile.h"
#include <vector>
#include <QComboBox>

using namespace std;

class ProfileManager
{
public:
    ProfileManager();
    Profile getByName(QString);
    bool addProfile(Profile);
    bool deleteByName(QString);
    void refreshList(QComboBox*);
    vector<Profile> profiles;
};

#endif // PROFILEMANAGER_H
