#include "UserResponse.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

void saveUserResponse(const UserResponse& response, const string& directory) {
    string responseDirectory = directory + "\\userResponse";

    if (!filesystem::exists(responseDirectory)) {
        filesystem::create_directories(responseDirectory);
    }

    // Create the file path using the username and survey title
    string responseFilePath = responseDirectory + "\\" + response.username + "_" + response.surveyTitle + ".bin";
    ofstream out(responseFilePath, ios::binary | ios::trunc);

    if (!out) {
        cout << "Error opening file for saving the user response." << endl;
        return;
    }

    // Write the username and survey title to the binary file
    out.write(response.username, sizeof(response.username));
    out.write(response.surveyTitle, sizeof(response.surveyTitle));

    // Write the number of answers
    int numAnswers = response.answers.size();
    out.write(reinterpret_cast<const char*>(&numAnswers), sizeof(numAnswers));

    // Write each answer
    for (const string& answer : response.answers) {
        int answerLength = answer.size();
        out.write(reinterpret_cast<const char*>(&answerLength), sizeof(answerLength));
        out.write(answer.c_str(), answerLength);
    }

    out.close();
    cout << "User response saved successfully!" << endl;
}