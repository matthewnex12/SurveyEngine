#ifndef SURVEY_H
#define SURVEY_H

#include <string>
#include <vector>

using namespace std;

struct Survey {
    char title[100];
    vector<string> questions;
    vector<vector<string>> answers;
};

void createSurvey();
void takeSurvey();
void viewSurvey();
void editSurvey();

#endif
