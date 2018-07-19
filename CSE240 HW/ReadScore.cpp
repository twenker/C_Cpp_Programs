//Todd Wenker
//CSE 240 Project ReadScore
#include<iostream>
#include<fstream>

using namespace std;

void readScore(){
    ifstream myfile ("score.csv");
    string gameTitle, graphics, gameplay, story;
    int graphicsScore, gameplayScore, storyScore, counter = 0;
    if(myfile.is_open()){
        while(getline(myfile, gameTitle, ',')){
            getline(myfile, graphics, ',');
            getline(myfile, gameplay, ',');
            getline(myfile, story, ',');
            if(counter != 0){
                graphicsScore = stoi(graphics);
                gameplayScore = stoi(gameplay);
                storyScore = stoi(story);
                cout << gameTitle << endl;
                cout << "Graphics: " << graphicsScore << endl;
                cout << "Gameplay: " << gameplayScore << endl;
                cout << "Story: " << storyScore << endl;
            }
            counter++;
        }
        myfile.close();
    }
    else{
        cout << "Unable to read score.csv" << endl;
    }
    return 0;

}
