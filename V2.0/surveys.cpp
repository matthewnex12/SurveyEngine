#include "surveys.h"
#include "UserResponse.h"
#include "User.h"
#include "UserSession.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <limits>

using namespace std;

vector<Survey> loadAllSurveys(const string& directory) {
    vector<Survey> surveys;

    for (const auto& entry : filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".bin" && entry.path().stem() != "users") {
            Survey survey;
            ifstream in(entry.path(), ios::binary);
            if (!in) continue;

            // Read survey title
            in.read(survey.title, sizeof(survey.title));

            // Read number of questions
            int numQuestions = 0;
            in.read(reinterpret_cast<char*>(&numQuestions), sizeof(numQuestions));

            for (int i = 0; i < numQuestions; ++i) {
                int questionLength = 0;
                in.read(reinterpret_cast<char*>(&questionLength), sizeof(questionLength));

                string question(questionLength, ' ');
                in.read(&question[0], questionLength);
                survey.questions.push_back(question);

                // Read number of answers
                int numAnswers = 0;
                in.read(reinterpret_cast<char*>(&numAnswers), sizeof(numAnswers));

                vector<string> answers;
                for (int j = 0; j < numAnswers; ++j) {
                    int answerLength = 0;
                    in.read(reinterpret_cast<char*>(&answerLength), sizeof(answerLength));

                    string answer(answerLength, ' ');
                    in.read(&answer[0], answerLength);
                    answers.push_back(answer);
                }
                survey.answers.push_back(answers);
            }
            surveys.push_back(survey);
            in.close();
        }
    }

    return surveys;
}

void saveSurvey(const Survey& survey, const string& directory) {
    string surveyFilePath = directory + "\\" + survey.title + ".bin";
    ofstream out(surveyFilePath, ios::binary | ios::trunc);
    if (!out) {
        cout << "Error opening file for saving the survey." << endl;
        return;
    }

    // Write survey title
    out.write(survey.title, sizeof(survey.title));

    // Write number of questions
    int numQuestions = survey.questions.size();
    out.write(reinterpret_cast<const char*>(&numQuestions), sizeof(numQuestions));

    for (size_t i = 0; i < survey.questions.size(); ++i) {
        int questionLength = survey.questions[i].size();
        out.write(reinterpret_cast<const char*>(&questionLength), sizeof(questionLength));
        out.write(survey.questions[i].c_str(), questionLength);

        int numAnswers = survey.answers[i].size();
        out.write(reinterpret_cast<const char*>(&numAnswers), sizeof(numAnswers));
        for (const string& answer : survey.answers[i]) {
            int answerLength = answer.size();
            out.write(reinterpret_cast<const char*>(&answerLength), sizeof(answerLength));
            out.write(answer.c_str(), answerLength);
        }
    }

    out.close();
}




void createSurvey() {
    // Get survey title
    string title;
    cout << "Enter the title of the survey: ";
    getline(cin >> ws, title);

    // Validate title length
    if (title.length() >= 100) {
        cout << "Error: Title is too long. Maximum length is 99 characters." << endl;
        return;
    }

    // Get the number of questions
    int numQuestions;
    cout << "Enter the number of questions in the survey: ";
    cin >> numQuestions;

    Survey survey;
    strncpy(survey.title, title.c_str(), sizeof(survey.title) - 1);
    survey.title[sizeof(survey.title) - 1] = '\0';

    // Loop through questions
    for (int i = 0; i < numQuestions; ++i) {
        string question;
        cout << "Question (" << i + 1 << "): ";
        getline(cin >> ws, question);
        survey.questions.push_back(question);

        // Get the number of answers
        int numAnswers;
        cout << "Enter the number of answers for this question: ";
        cin >> numAnswers;

        vector<string> answers;
        for (int j = 0; j < numAnswers; ++j) {
            string answer;
            cout << "Answer " << j + 1 << ": ";
            getline(cin >> ws, answer);
            answers.push_back(answer);
        }
        survey.answers.push_back(answers);
    }

    // Ensure the directory exists
    string dirPath = "Z:\\CIS 17B\\individualProject\\DB";
    if (!filesystem::exists(dirPath)) {
        filesystem::create_directories(dirPath);
    }

    // Save the survey in a binary file with title
    string surveyFilePath = dirPath + "\\" + title + ".bin";
    ofstream out(surveyFilePath, ios::binary | ios::trunc);
    if (!out) {
        cout << "Error opening file for saving the survey." << endl;
        return;
    }

    // Write the survey title
    out.write(survey.title, sizeof(survey.title));

    // Write the number of questions
    int numQuestionsToWrite = survey.questions.size();
    out.write(reinterpret_cast<const char*>(&numQuestionsToWrite), sizeof(numQuestionsToWrite));

    // Write each question and its answers
    for (size_t i = 0; i < survey.questions.size(); ++i) {
        int questionLength = survey.questions[i].size();
        out.write(reinterpret_cast<const char*>(&questionLength), sizeof(questionLength));
        out.write(survey.questions[i].c_str(), questionLength);

        int numAnswersToWrite = survey.answers[i].size();
        out.write(reinterpret_cast<const char*>(&numAnswersToWrite), sizeof(numAnswersToWrite));
        for (const string& answer : survey.answers[i]) {
            int answerLength = answer.size();
            out.write(reinterpret_cast<const char*>(&answerLength), sizeof(answerLength));
            out.write(answer.c_str(), answerLength);
        }
    }

    out.close();
    cout << "Survey saved successfully!" << endl;
}

void takeSurvey() {
    string surveyDirectory = "Z:\\CIS 17B\\individualProject\\DB";
    vector<Survey> surveys = loadAllSurveys(surveyDirectory);

    if (surveys.empty()) {
        cout << "No surveys available to take." << endl;
        return;
    }

    // Display all surveys for selection
    cout << "Choose a survey to take:" << endl;
    for (size_t i = 0; i < surveys.size(); ++i) {
        cout << i + 1 << ". " << surveys[i].title << endl;
    }

    // Get the user's selection
    size_t surveyIndex = 0;
    cout << "Enter the number corresponding to the survey: ";
    cin >> surveyIndex;

    if (surveyIndex == 0 || surveyIndex > surveys.size()) {
        cout << "Invalid choice." << endl;
        return;
    }

    // Access the selected survey
    Survey& selectedSurvey = surveys[surveyIndex - 1];
    UserResponse response;
    strncpy(response.username, UserSession::getInstance().getCurrentUser().username, sizeof(response.username) - 1);
    strncpy(response.surveyTitle, selectedSurvey.title, sizeof(response.surveyTitle) - 1);
    response.username[sizeof(response.username) - 1] = '\0';
    response.surveyTitle[sizeof(response.surveyTitle) - 1] = '\0';

    // Go through each question in the selected survey
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for (size_t i = 0; i < selectedSurvey.questions.size(); ++i) {
        cout << "Question " << i + 1 << ": " << selectedSurvey.questions[i] << endl;
        for (size_t j = 0; j < selectedSurvey.answers[i].size(); ++j) {
            cout << j + 1 << ". " << selectedSurvey.answers[i][j] << endl;
        }

        string answer;
        cout << "Enter your answer: ";
        getline(cin, answer);
        response.answers.push_back(answer);
    }

    // Save the user's response
    saveUserResponse(response, surveyDirectory);
}

void viewSurveys() {
    string surveyDirectory = "Z:\\CIS 17B\\individualProject\\DB";

    // Check if the directory exists
    if (!filesystem::exists(surveyDirectory)) {
        cout << "Survey directory does not exist." << endl;
        return;
    }

    // find all `.bin` files excluding user files
    cout << "Available surveys:" << endl;
    bool foundSurveys = false;

    for (const auto& entry : filesystem::directory_iterator(surveyDirectory)) {
        // Check for `.bin` files that are not named `users`
        if (entry.path().extension() == ".bin" && entry.path().stem() != "users") {
            cout << entry.path().stem().string() << endl;
            foundSurveys = true;
        }
    }

    if (!foundSurveys) {
        cout << "No surveys found in the directory." << endl;
    }
}

void editSurvey() {
    string surveyDirectory = "Z:\\CIS 17B\\individualProject\\DB";
    vector<Survey> surveys = loadAllSurveys(surveyDirectory);

    if (surveys.empty()) {
        cout << "No surveys available to edit." << endl;
        return;
    }

    // Display all surveys for selection
    cout << "Choose a survey to edit:" << endl;
    for (size_t i = 0; i < surveys.size(); ++i) {
        cout << i + 1 << ". " << surveys[i].title << endl;
    }

    // Get the admin's selection
    size_t surveyIndex = 0;
    cout << "Enter the number corresponding to the survey: ";
    cin >> surveyIndex;

    if (surveyIndex == 0 || surveyIndex > surveys.size()) {
        cout << "Invalid choice." << endl;
        return;
    }

    // Access the selected survey
    Survey& selectedSurvey = surveys[surveyIndex - 1];

    // Display questions and let the admin choose which one to edit
    cout << "Choose a question to edit from " << selectedSurvey.title << ":\n";
    for (size_t i = 0; i < selectedSurvey.questions.size(); ++i) {
        cout << i + 1 << ". " << selectedSurvey.questions[i] << endl;
    }

    size_t questionIndex = 0;
    cout << "Enter the number corresponding to the question: ";
    cin >> questionIndex;

    if (questionIndex == 0 || questionIndex > selectedSurvey.questions.size()) {
        cout << "Invalid choice." << endl;
        return;
    }

    // Modify the selected question and its answers
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string newQuestion;
    cout << "Enter the new question: ";
    getline(cin, newQuestion);
    selectedSurvey.questions[questionIndex - 1] = newQuestion;

    // Update answers for this question
    int numAnswers = 0;
    cout << "Enter the number of answers: ";
    cin >> numAnswers;

    vector<string> newAnswers;
    for (int j = 0; j < numAnswers; ++j) {
        string newAnswer;
        cout << "Answer " << j + 1 << ": ";
        getline(cin >> ws, newAnswer);
        newAnswers.push_back(newAnswer);
    }
    selectedSurvey.answers[questionIndex - 1] = newAnswers;

    // Save the modified survey
    saveSurvey(selectedSurvey, surveyDirectory);
    cout << "Survey updated successfully!" << endl;
}
