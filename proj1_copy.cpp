//MARISA KELLEY PROJECT 1 SECTION 009
#include <fstream> //https://stackoverflow.com/questions/13321644/c-using-fstream
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set> //https://www.youtube.com/watch?v=nfKVyv_up34
#include <iomanip>       //https://www.geeksforgeeks.org/iomanip-in-cpp/
#include <map>

// Function to calculate the mean of a vector of doubles
double calculateMean(const std::vector<double> &ratings)
{
  if (ratings.empty())
    return 0.0; // error handling
  double sum = 0.0;
  for (double rating : ratings)
  {
    sum += rating;
  }
  return sum / ratings.size();
}

// function to calculate the standard deviation with a vector of doubles
double calculateStdDev(const std::vector<double> &ratings, double mean)
{
  if (ratings.size() <= 1)
    return 0.0; // handles a single-entry case
  double variance = 0.0;
  for (double rating : ratings)
  {
    variance += std::pow(rating - mean, 2);
  }
  return std::sqrt(variance / ratings.size());
}

// function to find the min value in a vector of doubles
double findMin(const std::vector<double> &ratings)
{
  if (ratings.empty())
    return 0.0;
  return *std::min_element(ratings.begin(), ratings.end());
}

// function to find the maximum value in a vector of doubles
double findMax(const std::vector<double> &ratings)
{
  if (ratings.empty())
    return 0.0;
  return *std::max_element(ratings.begin(), ratings.end());
}

// function to classify review as positive/negative/inconclusive
std::string classifyReview(const std::string &review,
                           const std::vector<std::string> &positiveWords,
                           const std::vector<std::string> &negativeWords)
{
  std::unordered_set<std::string> posSet(positiveWords.begin(), positiveWords.end()); // https://www.youtube.com/watch?v=nfKVyv_up34
  std::unordered_set<std::string> negSet(negativeWords.begin(), negativeWords.end());

  int posCount = 0, negCount = 0;
  std::stringstream ss(review);
  std::string word;
  while (ss >> word)
  {
    // removes punctuation from words
    word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
    // makes all words lowercase
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (posSet.find(word) != posSet.end())
    {
      posCount++;
    }
    if (negSet.find(word) != negSet.end())
    {
      negCount++;
    }
  }

  if (posCount > negCount)
    return "Positive";
  if (negCount > posCount)
    return "Negative";
  return "Inconclusive";
}

// function to display the comparison results
void displayResults(const std::vector<double> &ratings1,
                    const std::vector<double> &ratings2,
                    const std::vector<std::string> &reviews1,
                    const std::vector<std::string> &reviews2,
                    const std::vector<std::string> &positiveWords,
                    const std::vector<std::string> &negativeWords,
                    const std::vector<std::string> &titles1,
                    const std::vector<std::string> &titles2)
{
  // calculate statistics for dataset 1
  double mean1 = calculateMean(ratings1);
  double stdDev1 = calculateStdDev(ratings1, mean1);
  double min1 = findMin(ratings1);
  double max1 = findMax(ratings1);

  // calculate statistics for dataset 2
  double mean2 = calculateMean(ratings2);
  double stdDev2 = calculateStdDev(ratings2, mean2);
  double min2 = findMin(ratings2);
  double max2 = findMax(ratings2);

  // performing sentiment analysis for second first of data
  int pos1 = 0, neg1 = 0, inc1 = 0;
  for (const std::string &review : reviews1)
  {
    std::string sentiment = classifyReview(review, positiveWords, negativeWords);
    if (sentiment == "Positive")
      pos1++;
    else if (sentiment == "Negative")
      neg1++;
    else
      inc1++;
  }

  // performing sentiment analysis for second set of data
  int pos2 = 0, neg2 = 0, inc2 = 0;
  for (const std::string &review : reviews2)
  {
    std::string sentiment = classifyReview(review, positiveWords, negativeWords);
    if (sentiment == "Positive")
      pos2++;
    else if (sentiment == "Negative")
      neg2++;
    else
      inc2++;
  }

  // finding the overall best and worst movies
  std::map<std::string, double> movieRatings;
  for (size_t i = 0; i < titles1.size(); i++)
  {
    movieRatings[titles1[i]] = ratings1[i];
  }
  for (size_t i = 0; i < titles2.size(); i++)
  {
    movieRatings[titles2[i]] = ratings2[i];
  }

  std::string bestMovie, worstMovie;
  double bestRating = 0.0, worstRating = 10.0;
  for (const auto &entry : movieRatings)
  {
    if (entry.second > bestRating)
    {
      bestRating = entry.second;
      bestMovie = entry.first;
    }
    if (entry.second < worstRating)
    {
      worstRating = entry.second;
      worstMovie = entry.first;
    }
  }

  // displays the results
  std::cout << std::fixed << std::setprecision(2); // lab 1
  std::cout << "\t\tSet 1\t\tSet 2\n";
  std::cout << "------------------------------------------------\n";
  std::cout << "Count:  \t" << ratings1.size() << "\t\t" << ratings2.size() << "\n";
  std::cout << "Mean:   \t" << mean1 << "\t\t" << mean2 << "\n";
  std::cout << "STDV:   \t" << stdDev1 << "\t\t" << stdDev2 << "\n";
  std::cout << "Min:    \t" << min1 << "\t\t" << min2 << "\n";
  std::cout << "Max:    \t" << max1 << "\t\t" << max2 << "\n";
  std::cout << "Pos:    \t" << pos1 << "\t\t" << pos2 << "\n";
  std::cout << "Neg:    \t" << neg1 << "\t\t" << neg2 << "\n";
  std::cout << "Inc:    \t" << inc1 << "\t\t" << inc2 << "\n";
  std::cout << "Overall Best Title: " << bestMovie << "\n";
  std::cout << "Overall Worst Title: " << worstMovie << "\n";
}

// loads in the dictionary.txt file into two vectors of positiveWords and negativeWords
bool load_dictionary(const std::string &filename,
                     std::vector<std::string> &positiveWords,
                     std::vector<std::string> &negativeWords)
{
  std::ifstream file(filename); ////https://stackoverflow.com/questions/13321644/c-using-fstream
  std::string line;

  // reads the positive words
  if (std::getline(file, line))
  {
    const char delimiter{','};
    std::string token;
    std::stringstream posStream(line);
    while (std::getline(posStream, token, delimiter))
    {
      positiveWords.push_back(token);
    }
  }

  // reads the negative words
  if (std::getline(file, line))
  {
    const char delimiter{','};
    std::string token;
    std::stringstream negStream(line);
    while (std::getline(negStream, token, delimiter))
    {
      negativeWords.push_back(token);
    }
  }

  // checking if dictionary is empty
  if (positiveWords.empty() || negativeWords.empty())
  {
    return false; // was empty
  }

  return true; // wasnt empty
}

// loads the dataset into tokens representing each column
void load_dataset(const std::string &filename, std::vector<std::string> &titles,
                  std::vector<int> &years, std::vector<double> &ratings,
                  std::vector<std::string> &reviews)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error opening dataset file: " << filename << std::endl; // error handling
    return;
  }
  std::string line, title, review;
  int year;
  double rating;
  std::string temp; // skips header line
  getline(file, line);

  while (getline(file, line))
  {
    std::stringstream ss(line);

    getline(ss, title, ',');
    getline(ss, temp, ',');
    year = std::stoi(temp);
    getline(ss, temp, ',');
    rating = std::stod(temp);
    getline(ss, review);

    // treats each line as a separate entry
    titles.push_back(title);
    years.push_back(year);
    ratings.push_back(rating);
    reviews.push_back(review);
  }

  file.close();
}

// main function
int main()
{
  // loads in the different files and data that i need
  const std::string dataset1_path{"set1.csv"};
  const std::string dataset2_path{"set2.csv"};
  const std::string dictionary_path{"dictionary.txt"};

  // loads the word dictionary
  std::vector<std::string> positiveWords, negativeWords;
  if (!load_dictionary(dictionary_path, positiveWords, negativeWords))
  {
    std::cerr << "Error: Dictionary file is empty or improperly formatted. Exiting program." << std::endl;
    return 1; // stops the programm
  }

  // loads the datasets
  std::vector<std::string> titles1, titles2;
  std::vector<int> years1, years2;
  std::vector<double> ratings1, ratings2;
  std::vector<std::string> reviews1, reviews2;

  load_dataset(dataset1_path, titles1, years1, ratings1, reviews1);
  load_dataset(dataset2_path, titles2, years2, ratings2, reviews2);

  // checking if one/both datasets are empty
  if (titles1.empty() || titles2.empty())
  {
    std::cerr << "Error: One or both datasets are empty. Exiting program." << std::endl;
    return 1; // stops the program
  }

  // displays the results
  displayResults(ratings1, ratings2, reviews1, reviews2, positiveWords, negativeWords, titles1, titles2);

  return 0;
}