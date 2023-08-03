#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define LOW_TO_HIGH 0
#define HIGH_TO_LOW 1

#define WIDTH 1200
#define HEIGHT 1050

using namespace std;

struct House {
    string _city;
    string _state;
    int _numOfBeds;
    double _value;
};

vector<House>filteredData;

class houseSearch {
private:
    vector<House> houseData;
public:
    void readFile();
    vector<House> data(int numOfBeds, string state, double minValue, double maxValue);

    int cityPartition(vector<string>& data, int low, int high, int sortType);
    void cityQuickSort(vector<string>& data, int low, int high, int sortType);
    int pricePartition(vector<double>& data, int low, int high, int sortType);
    void priceQuickSort(vector<double>& data, int low, int high, int sortType);

    void cityMerge(vector<string>& data, vector<string>& left, vector<string>& right, int sortType);
    void cityMergeSort(vector<string>& data, int sortType);
    void priceMerge(vector<double>& data, vector<double>& left, vector<double>& right, int sortType);
    void priceMergeSort(vector<double>& data, int sortType);
};

void houseSearch::readFile() {
    ifstream inputFile("data/house.csv");

    if (!inputFile.is_open()) {
        cout << "Error!" << endl;
        return;
    }
    string line;
    // read the header
    getline(inputFile, line);

    while(getline(inputFile, line)) {
        istringstream iss(line);
        House house;

        getline(iss, house._city, ',');
        getline(iss, house._state, ',');
        string numStr;
        getline(iss, numStr, ',');
        try {
            house._numOfBeds = stoi(numStr);
        } catch (const std::invalid_argument& e) {
            cout << "Error: Invalid number of beds in the data file." << endl;
            continue;
        }

        string valStr;
        getline(iss, valStr);
        try {
            house._value = stod(valStr);
        } catch (const std::invalid_argument& e) {
            cout << "Error: Invalid value in the data file." << endl;
            continue;
        }

        houseData.push_back(house);
    }
    inputFile.close();
}

vector<House> houseSearch::data(int numOfBeds, string state, double minValue, double maxValue) {
    vector<House> filteredData;

    for (const auto& house: houseData) {
        if (house._numOfBeds == numOfBeds && house._state == state && house._value >= minValue && house._value <= maxValue) {
            filteredData.push_back(house);
        }
    }
    if (filteredData.empty()) {
        cout << "No information" << endl;
    }
    return filteredData;
}

int houseSearch::cityPartition(vector<string>& data, int low, int high, int sortType) {
    string pivot = data[low];
    int up = low + 1;
    int down = high;

    switch (sortType) {
        case 0:
            while (up <= down) {
                while (up <= down && data[up] <= pivot) {
                    up++;
                }
                while (data[down] > pivot) {
                    down--;
                }
                if (up < down) {
                    swap(data[up], data[down]);
                }
            }
            swap(data[low], data[down]);
            return down;

        case 1:
            while (up <= down) {
                while (up <= down && data[up] >= pivot) {
                    up++;
                }
                while (data[down] < pivot) {
                    down--;
                }
                if (up < down) {
                    swap(data[up], data[down]);
                }
            }
            swap(data[low], data[down]);
            return down;
    }
}

void houseSearch::cityQuickSort(vector<string>& data, int low, int high, int sortType) {
    if (low < high) {
        int pivot = cityPartition(data, low, high, sortType);
        cityQuickSort(data, low, pivot - 1, sortType);
        cityQuickSort(data, pivot + 1, high, sortType);
    }
}

int houseSearch::pricePartition(vector<double>& data, int low, int high, int sortType) {
    double pivot = data[low];
    int up = low + 1;
    int down = high;

    switch (sortType) {
        case 0:
            while (up <= down) {
                while (up <= down && data[up] <= pivot) {
                    up++;
                }
                while (data[down] > pivot) {
                    down--;
                }
                if (up < down) {
                    swap(data[up], data[down]);
                }
            }
            swap(data[low], data[down]);
            return down;

        case 1:
            while (up <= down) {
                while (up <= down && data[up] >= pivot) {
                    up++;
                }
                while (data[down] < pivot) {
                    down--;
                }
                if (up < down) {
                    swap(data[up], data[down]);
                }
            }
            swap(data[low], data[down]);
            return down;
    }
}

void houseSearch::priceQuickSort(vector<double>& data, int low, int high, int sortType) {
    if (low < high) {
        int pivot = pricePartition(data, low, high, sortType);
        priceQuickSort(data, low, pivot - 1, sortType);
        priceQuickSort(data, pivot + 1, high, sortType);
    }
}

void houseSearch::cityMerge(vector<string> &data, vector<string> &left, vector<string> &right, int sort_type) {
    int leftIndex = 0;
    int rightIndex = 0;
    int dataIndex = 0;

    while (leftIndex < left.size() && rightIndex < right.size()) {
        if ((sort_type == 0 && left[leftIndex] <= right[rightIndex]) || (sort_type == 1 && left[leftIndex] >= right[rightIndex])) {
            data[dataIndex] = left[leftIndex];
            leftIndex++;
        } else {
            data[dataIndex] = right[rightIndex];
            rightIndex++;
        }
        dataIndex++;
    }

    while (leftIndex < left.size()) {
        data[dataIndex] = left[leftIndex];
        dataIndex++;
        leftIndex++;
    }

    while (rightIndex < right.size()) {
        data[dataIndex] = right[rightIndex];
        dataIndex++;
        rightIndex++;
    }
}

void houseSearch::cityMergeSort(vector<string> &data, int sort_type) {
    if (data.size() <= 1) {
        return;
    }

    int mid = data.size() / 2;
    vector<string> left(data.begin(), data.begin() + mid);
    vector<string> right(data.begin() + mid, data.end());

    cityMergeSort(left, sort_type);
    cityMergeSort(right, sort_type);

    cityMerge(data, left, right, sort_type);
}

void houseSearch::priceMerge(vector<double> &data, vector<double> &left, vector<double> &right, int sort_type) {
    int leftIndex = 0;
    int rightIndex = 0;
    int dataIndex = 0;

    while (leftIndex < left.size() && rightIndex < right.size()) {
        if ((sort_type == 0 && left[leftIndex] <= right[rightIndex]) || (sort_type == 1 && left[leftIndex] >= right[rightIndex])) {
            data[dataIndex] = left[leftIndex];
            leftIndex++;
        } else {
            data[dataIndex] = right[rightIndex];
            rightIndex++;
        }
        dataIndex++;
    }

    while (leftIndex < left.size()) {
        data[dataIndex] = left[leftIndex];
        dataIndex++;
        leftIndex++;
    }

    while (rightIndex < right.size()) {
        data[dataIndex] = right[rightIndex];
        dataIndex++;
        rightIndex++;
    }
}

void houseSearch::priceMergeSort(vector<double> &data, int sort_type) {
    if (data.size() <= 1) {
        return;
    }

    int mid = data.size() / 2;
    std::vector<double> left(data.begin(), data.begin() + mid);
    std::vector<double> right(data.begin() + mid, data.end());

    priceMergeSort(left, sort_type);
    priceMergeSort(right, sort_type);

    priceMerge(data, left, right, sort_type);
}

void generate_start_window() {

    sf::Font font;
    font.loadFromFile("data/font.ttf");

    sf::Text welcomeText("HOUSEARCH", font, 80);
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setStyle(sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = welcomeText.getGlobalBounds();
    float welcomeTextWidth = WIDTH / 2 - textRect.width / 2;
    float welcomeTextHeight = HEIGHT / 2 - textRect.height / 2 - 200;
    welcomeText.setPosition(welcomeTextWidth, welcomeTextHeight);

    sf::Text line("Find the house of your dream", font, 30);
    line.setFillColor(sf::Color::Black);
    sf::FloatRect lineRect = line.getGlobalBounds();
    float lineWidth = WIDTH / 2 - lineRect.width / 2;
    float lineHeight = HEIGHT / 2 - lineRect.height / 2 - 75;
    line.setPosition(lineWidth, lineHeight);

    sf::Text startButton("Start", font, 30);
    startButton.setFillColor(sf::Color::Black);
    sf::FloatRect buttonRect = startButton.getGlobalBounds();
    float buttonWidth = WIDTH / 2 - buttonRect.width / 2;
    float buttonHeight = HEIGHT / 2 - buttonRect.height / 2 + 50;
    startButton.setPosition(buttonWidth, buttonHeight);

    sf::RectangleShape box;
    box.setSize(sf::Vector2f(200, 100));
    box.setFillColor(sf::Color(104, 109, 117));
    box.setPosition(buttonWidth - 50, buttonHeight - 33);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Housearch");
    window.clear(sf::Color(220, 220, 220)); // Light gray color

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        window.close();
                    }
                }
            }
        }
        window.clear(sf::Color(220, 220, 220));
        window.draw(welcomeText);
        window.draw(line);
        window.draw(box);
        window.draw(startButton);
        window.display();
    }
}


int main() {

    generate_start_window();

    string state;
    int numOfBeds;
    double minPrice;
    double maxPrice;
    vector <double> prices;
    vector<string> city;

    houseSearch hou;
    hou.readFile();

    sf::RenderWindow inputWindow(sf::VideoMode(WIDTH, HEIGHT), "HouSearch");
    inputWindow.clear(sf::Color(220, 220, 220));

    sf::Font font;
    font.loadFromFile("data/font.ttf");

    // items for sort Window
    sf::Text text("HOUSEARCH", font, 80);
    text.setStyle(sf::Text::Bold);
    text.setStyle(sf::Text::Underlined);
    text.setFillColor(sf::Color::Black);
    sf::FloatRect textRec = text.getGlobalBounds();
    float textWidth = WIDTH/2 - textRec.width/2;
    float textHeight = 0;
    text.setPosition(textWidth,textHeight);

    sf::Text lowToHigh("Low to High", font, 30);
    lowToHigh.setFillColor(sf::Color::Black);
    sf::FloatRect lowToHighRect = lowToHigh.getGlobalBounds();
    float lowToHighRectWidth = WIDTH/2 - lowToHighRect.width/2;
    float lowToHighRectHeight = HEIGHT/2 - lowToHighRect.height/2;
    lowToHigh.setPosition(lowToHighRectWidth + 300, lowToHighRectHeight - 383);

    sf::RectangleShape lowBox;
    lowBox.setSize(sf::Vector2f(350, 60));
    lowBox.setFillColor(sf::Color(138, 145, 156));
    lowBox.setPosition(lowToHighRectWidth + 220, lowToHighRectWidth - 385);

    sf::Text highToLow("High to Low", font, 30);
    highToLow.setFillColor(sf::Color::Black);
    sf::FloatRect highToLowRect = highToLow.getGlobalBounds();
    float highToLowRectWidth = WIDTH/2 - highToLowRect.width/2;
    float highToLowRectHeight = HEIGHT/2 - highToLowRect.height/2;
    highToLow.setPosition(highToLowRectWidth - 305, highToLowRectHeight - 382);

    sf::RectangleShape highBox;
    highBox.setSize(sf::Vector2f(350, 60));
    highBox.setFillColor(sf::Color(138, 145, 156));
    highBox.setPosition(highToLowRectWidth - 380, highToLowRectHeight - 391);

    sf::RectangleShape mergeBox;
    mergeBox.setSize(sf::Vector2f(580, 750));
    mergeBox.setFillColor(sf::Color(138, 145, 156));
    mergeBox.setPosition(10,200);

    sf::RectangleShape quickBox;
    quickBox.setSize(sf::Vector2f(580, 750));
    quickBox.setFillColor(sf::Color(138, 145, 156));
    quickBox.setPosition(610, 200);

    // item for input Window
    sf::Text welcomeText("HOUSEARCH", font, 80);
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setStyle(sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = welcomeText.getGlobalBounds();
    float welcomeTextWidth = WIDTH/2 - textRect.width/2;
    float welcomeTextHeight = HEIGHT - 950;
    welcomeText.setPosition(welcomeTextWidth,welcomeTextHeight);

    sf::Text line("Enter Criteria: ", font, 30);
    line.setFillColor(sf::Color::Black);
    line.setStyle(sf::Text::Italic);
    sf::FloatRect lineRect = line.getGlobalBounds();
    float lineWidth = WIDTH/2 - lineRect.width/2;
    float lineHeight = HEIGHT/2 - lineRect.height/2 - 200;
    line.setPosition(lineWidth,lineHeight);

    sf::Text numOfBedsTitle("Number of Beds (from 1 to 5):", font, 20);
    numOfBedsTitle.setFillColor(sf::Color::Black);
    sf::FloatRect numOfBedsRect = numOfBedsTitle.getGlobalBounds();
    float numOfBedsRectWidth = WIDTH/2 - numOfBedsRect.width/2 - 200;
    float numOfBedsRectHeight = HEIGHT/2 - numOfBedsRect.height/2 - 100;
    numOfBedsTitle.setPosition(numOfBedsRectWidth, numOfBedsRectHeight);

    sf::RectangleShape box1;
    box1.setSize(sf::Vector2f(400, 60));
    box1.setFillColor(sf::Color(138, 145, 156));
    box1.setPosition(numOfBedsRectWidth + 380, numOfBedsRectHeight - 20);

    sf::Text stateTitle("State (Capital Abbreviation): ", font, 20);
    stateTitle.setFillColor(sf::Color::Black);
    sf::FloatRect stateInputRect = stateTitle.getGlobalBounds();
    float stateInputRectWidth = WIDTH/2 - stateInputRect.width/2 - 180;
    float stateInputRectHeight = HEIGHT/2 - stateInputRect.height/2;
    stateTitle.setPosition(stateInputRectWidth, stateInputRectHeight);

    sf::RectangleShape box2;
    box2.setSize(sf::Vector2f(400, 60));
    box2.setFillColor(sf::Color(138, 145, 156));
    box2.setPosition(stateInputRectHeight + 92, stateInputRectHeight - 20);

    sf::Text minPriceTitle("Minimum Price: ", font, 20);
    minPriceTitle.setFillColor(sf::Color::Black);
    sf::FloatRect minPriceInputRect = minPriceTitle.getGlobalBounds();
    float minPriceInputRectWidth = WIDTH/2 - minPriceInputRect.width/2 - 180;
    float minPriceInputRectHeight = HEIGHT/2 - minPriceInputRect.height/2 + 100;
    minPriceTitle.setPosition(minPriceInputRectWidth, minPriceInputRectHeight);

    sf::RectangleShape box3;
    box3.setSize(sf::Vector2f(400, 60));
    box3.setFillColor(sf::Color(138, 145, 156));
    box3.setPosition(minPriceInputRectWidth + 280, minPriceInputRectHeight - 20);

    sf::Text maxPriceTitle("Maximum Price: ", font, 20);
    maxPriceTitle.setFillColor(sf::Color::Black);
    sf::FloatRect maxPriceInputRect = maxPriceTitle.getGlobalBounds();
    float maxPriceInputRectWidth = WIDTH/2 - maxPriceInputRect.width/2 - 180;
    float maxPriceInputRectHeight = HEIGHT/2 - maxPriceInputRect.height/2 + 200;
    maxPriceTitle.setPosition(maxPriceInputRectWidth, maxPriceInputRectHeight);

    sf::RectangleShape box4;
    box4.setSize(sf::Vector2f(400, 60));
    box4.setFillColor(sf::Color(138, 145, 156));
    box4.setPosition(maxPriceInputRectWidth + 280, maxPriceInputRectHeight - 20);

    sf::Text maxInput("", font, 25);
    maxInput.setFillColor(sf::Color::White);
    maxInput.setPosition(maxPriceInputRectWidth + 400, maxPriceInputRectHeight - 5);

    sf::Text minInput("", font, 25);
    minInput.setFillColor(sf::Color::White);
    minInput.setPosition(minPriceInputRectWidth + 400, minPriceInputRectHeight - 5);

    sf::Text numInput("", font, 25);
    numInput.setFillColor(sf::Color::White);
    numInput.setPosition(numOfBedsRectWidth + 530, numOfBedsRectHeight - 5);

    sf::Text stateInput("", font, 25);
    stateInput.setFillColor(sf::Color::White);
    stateInput.setPosition(stateInputRectWidth + 520, stateInputRectHeight - 5);

    sf::Text priceSearch("Search by Price ", font, 30);
    priceSearch.setFillColor(sf::Color::Black);
    sf::FloatRect priceSearchRect = priceSearch.getGlobalBounds();
    float priceSearchRectWidth = WIDTH/2 - priceSearchRect.width/2 + 195;
    float priceSearchRectHight = HEIGHT/2 - priceSearchRect.height/2 + 380;
    priceSearch.setPosition(priceSearchRectWidth, priceSearchRectHight);

    sf::RectangleShape priceSearchBox;
    priceSearchBox.setSize(sf::Vector2f(350, 70));
    priceSearchBox.setFillColor(sf::Color(138, 145, 156));
    priceSearchBox.setPosition(priceSearchRectWidth - 40, priceSearchRectWidth + 222);

    sf::Text citySearch("Search by City ", font, 30);
    citySearch.setFillColor(sf::Color::Black);
    sf::FloatRect citySearchRect = citySearch.getGlobalBounds();
    float citySearchRectWidth = WIDTH/2 - citySearchRect.width/2 - 200 ;
    float citySearchRectHeight = HEIGHT/2 - citySearchRect.height/2 + 380;
    citySearch.setPosition(citySearchRectWidth, citySearchRectHeight);

    sf::RectangleShape citySearchBox;
    citySearchBox.setSize(sf::Vector2f(350, 70));
    citySearchBox.setFillColor(sf::Color(138, 145, 156));
    citySearchBox.setPosition(citySearchRectWidth - 45, citySearchRectHeight - 13);


    bool isBox1Clicked = false;
    bool isBox2Clicked = false;
    bool isBox3Clicked = false;
    bool isBox4Clicked = false;
    bool dataFiltered = false;
    bool clickBox = false;
    bool searchByCity = false;
    bool searchByPrice = false;


    while (inputWindow.isOpen()) {
        sf::Event newEvent;
        while (inputWindow.pollEvent(newEvent)) {
            if (newEvent.type == sf::Event::Closed) {
                inputWindow.close();
            }

            if (newEvent.type == sf::Event::MouseButtonPressed) {
                if (box4.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                    isBox4Clicked = true;
                } else {
                    isBox4Clicked = false;
                }
                if (box3.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                    isBox3Clicked = true;
                } else {
                    isBox3Clicked = false;
                }
                if (box2.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                    isBox2Clicked = true;
                } else {
                    isBox2Clicked = false;
                }
                if (box1.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                    isBox1Clicked = true;
                } else {
                    isBox1Clicked = false;
                }
            }

            if (newEvent.type == sf::Event::TextEntered && isBox1Clicked) {
                if (newEvent.text.unicode >= 49 && newEvent.text.unicode <= 53) {
                    if (numInput.getString().getSize() < 1) {
                        numInput.setString(numInput.getString() + static_cast<char>(newEvent.text.unicode));
                        string str = numInput.getString();
                        numOfBeds = stoi(str);
                    }
                } else if (newEvent.text.unicode == 8 && numInput.getString().getSize() > 0) {
                    string str = numInput.getString();
                    str.pop_back();
                    numInput.setString(str);
                }
            }

            if (newEvent.type == sf::Event::TextEntered && isBox2Clicked) {
                if (newEvent.text.unicode >= 'A' && newEvent.text.unicode <= 'Z') {
                    if (stateInput.getString().getSize() < 2) {
                        stateInput.setString(stateInput.getString() + static_cast<char>(newEvent.text.unicode));
                        state = stateInput.getString();
                    }
                } else if (newEvent.text.unicode == 8 && stateInput.getString().getSize() > 0) {
                    string str = stateInput.getString();
                    str.pop_back();
                    stateInput.setString(str);
                }
            }

            if (newEvent.type == sf::Event::TextEntered && isBox3Clicked) {
                if (newEvent.text.unicode >= 48 && newEvent.text.unicode <= 57) {
                    minInput.setString(minInput.getString() + static_cast<char>(newEvent.text.unicode));
                    string str = minInput.getString();
                    minPrice = stod(str);
                } else if (newEvent.text.unicode == 8 && minInput.getString().getSize() > 0) {
                    string str = minInput.getString();
                    str.pop_back();
                    minInput.setString(str);
                }
            }

            if (newEvent.type == sf::Event::TextEntered && isBox4Clicked) {
                if (newEvent.text.unicode >= 48 && newEvent.text.unicode <= 57) {
                    maxInput.setString(maxInput.getString() + static_cast<char>(newEvent.text.unicode));
                    string str = maxInput.getString();
                    maxPrice = stod(str);
                } else if (newEvent.text.unicode == 8 && maxInput.getString().getSize() > 0) {
                    string str = maxInput.getString();
                    str.pop_back();
                    maxInput.setString(str);
                }
            }

            if (newEvent.type == sf::Event::MouseButtonPressed) {
                if (newEvent.mouseButton.button == sf::Mouse::Left) {
                    if (citySearchBox.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                        filteredData = hou.data(numOfBeds, state, minPrice, maxPrice);
                        for (const auto &house: filteredData) {
                            city.push_back(house._city);
                        }
                        dataFiltered = true;
                        searchByCity = true;
                        inputWindow.close();
                    } else if (priceSearchBox.getGlobalBounds().contains(newEvent.mouseButton.x,
                                                                         newEvent.mouseButton.y)) {
                        filteredData = hou.data(numOfBeds, state, minPrice, maxPrice);
                        for (const auto &house: filteredData) {
                            prices.push_back(house._value);
                        }
                        searchByPrice = true;
                        dataFiltered = true;
                        inputWindow.close();
                    }
                }
            }
        }

        inputWindow.clear(sf::Color(220, 220, 220));
        inputWindow.draw(welcomeText);
        inputWindow.draw(line);
        inputWindow.draw(numOfBedsTitle);
        inputWindow.draw(maxPriceTitle);
        inputWindow.draw(minPriceTitle);
        inputWindow.draw(stateTitle);
        inputWindow.draw(box1);
        inputWindow.draw(box2);
        inputWindow.draw(box3);
        inputWindow.draw(box4);
        inputWindow.draw(maxInput);
        inputWindow.draw(minInput);
        inputWindow.draw(numInput);
        inputWindow.draw(stateInput);
        inputWindow.draw(citySearchBox);
        inputWindow.draw(citySearch);
        inputWindow.draw(priceSearchBox);
        inputWindow.draw(priceSearch);

        inputWindow.display();
    }

    if (dataFiltered) {
        sf::RenderWindow sortWindow(sf::VideoMode(WIDTH, HEIGHT), "Housearch");
        sortWindow.clear(sf::Color(220, 220, 220)); // Light gray color

        while (sortWindow.isOpen()) {
            sf::Event e;
            while (sortWindow.pollEvent(e)) {
                if (e.type == sf::Event::Closed) {
                    sortWindow.close();
                }
                if (e.type == sf::Event::MouseButtonPressed) {
                    if (e.mouseButton.button == sf::Mouse::Left) {
                        if (searchByPrice) {
                            if (lowBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                clickBox = true;
                                hou.priceQuickSort(prices, 0, prices.size() - 1, LOW_TO_HIGH);
                                hou.priceMergeSort(prices, LOW_TO_HIGH);
                            }
                            if (highBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                clickBox = true;
                                hou.priceQuickSort(prices, 0, prices.size() - 1, HIGH_TO_LOW);
                                hou.priceMergeSort(prices, HIGH_TO_LOW);
                            }
                        } else if (searchByCity) {
                            if (lowBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                clickBox = true;
                                hou.cityQuickSort(city, 0, city.size() - 1, LOW_TO_HIGH);
                                hou.cityMergeSort(city, LOW_TO_HIGH);
                            }
                            if (highBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                clickBox = true;
                                hou.cityQuickSort(city, 0, city.size() - 1, HIGH_TO_LOW);
                                hou.cityMergeSort(city, HIGH_TO_LOW);
                            }
                        }
                    }
                }
            }


            sortWindow.clear(sf::Color(220, 220, 220));
            sortWindow.draw(mergeBox);
            sortWindow.draw(quickBox);
            if (clickBox) {
                float yOffset = 0.f;
                int count = 0;

                if (searchByPrice) {
                    for (const auto &price: prices) {
                        for (const auto &house: filteredData) {
                            if (house._value == price) {
                                sf::Text quickSortText("", font, 23);
                                quickSortText.setFillColor(sf::Color::Black);
                                std::stringstream ss;
                                ss << "- " << "City: " << house._city << ", Price: " << house._value;
                                quickSortText.setString(ss.str());
                                quickSortText.setPosition(620, 200 + yOffset);
                                sortWindow.draw(quickSortText);

                                sf::Text mergeSortText("", font, 23);
                                mergeSortText.setFillColor(sf::Color::Black);
                                std::stringstream sss;
                                sss << "- " << "City: " << house._city << ", Price: " << house._value;
                                mergeSortText.setString(sss.str());
                                mergeSortText.setPosition(10, 200 + yOffset);
                                sortWindow.draw(mergeSortText);
                                yOffset += mergeSortText.getLocalBounds().height + 3.f;
                                count++;

                            }
                        }
                        if (count >= 28) {
                            break;
                        }
                    }
                } else if (searchByCity) {
                    for (const auto &c: city) {
                        for (const auto &house: filteredData) {
                            if (house._city == c) {
                                sf::Text quickSortText("", font, 23);
                                quickSortText.setFillColor(sf::Color::Black);
                                std::stringstream ss;
                                ss << "- " << "City: " << house._city << ", Price: " << house._value;
                                quickSortText.setString(ss.str());
                                quickSortText.setPosition(620, 200 + yOffset);
                                sortWindow.draw(quickSortText);

                                sf::Text mergeSortText("", font, 23);
                                mergeSortText.setFillColor(sf::Color::Black);
                                std::stringstream sss;
                                sss << "- " << "City: " << house._city << ", Price: " << house._value;
                                mergeSortText.setString(sss.str());
                                mergeSortText.setPosition(10, 200 + yOffset);
                                sortWindow.draw(mergeSortText);
                                yOffset += mergeSortText.getLocalBounds().height + 3.f;
                                count++;
                            }
                        }
                        if (count >= 28) {
                            break;
                        }
                    }
                }
            }
            sortWindow.draw(text);
            sortWindow.draw(lowBox);
            sortWindow.draw(highBox);
            sortWindow.draw(lowToHigh);
            sortWindow.draw(highToLow);
            sortWindow.display();
        }
    }

    return 0;
}






