#include <iostream>
#include <cstdlib>
#include <opencv2\opencv.hpp>
#include <vector>
#include <random>
#include <assert.h>

const int APRIL_TAG_HEIGHT = 10;
const int APRIL_TAG_WIDTH = 10;
const int APRIL_TAG_BORDER_WIDTH = 1;
const int APRIL_TAG_PADDING_WIDTH = 1;
const int APRIL_TAG_PAYLOAD_WIDTH = 6;
//const unsigned long MAXIMUM_VALUE = 68719476736;
const unsigned long MAXIMUM_VALUE = 68719476735;
const int SD_WIDTH = 640;
const int SD_HEIGHT = 480;

int mainFunction();
void displayMenu();
void displayOptions(int startValue, int endValue, std::string filename);
void displayOptions(int startValue, int endValue, std::string filename, double mean, double standardDeviation, unsigned long numberOfImages);
bool saveImage(std::string filename, cv::Mat input);
std::vector<std::vector<bool>> generateArray(unsigned long input);
std::vector<std::vector<double>> generateImageValues(std::vector<std::vector<bool>> input);
cv::Mat generateImage(std::vector<std::vector<bool>> input);
void generateImage(std::vector<std::vector<bool>> input, cv::Mat& output);
std::vector<unsigned long> normalImages(unsigned long number);
std::vector<unsigned long> normalImages(unsigned long number, double mean, double standardDeviation);
void printHelp();

int main(int argc, char *argv[]) {
	return mainFunction();
}

int mainFunction() {
	int i, j, k;
	//cv::Mat aprilTag;
	double temporary;
	double pictureValues[APRIL_TAG_WIDTH][APRIL_TAG_WIDTH];
	std::vector<std::vector<double>> imageValues;
	std::string fileName;
	std::vector<unsigned long> values;
	unsigned long startValue;
	unsigned long endValue;
	unsigned long value;
	double mean = MAXIMUM_VALUE / 2;
	double standardDeviation = MAXIMUM_VALUE / 8;
	unsigned long numberOfImages;
	int choice;

	cv::namedWindow("AprilTag", cv::WINDOW_NORMAL);
	cv::resizeWindow("AprilTag", SD_WIDTH, SD_WIDTH);
	startValue = 0;
	endValue = 0;
	fileName = "output";
	numberOfImages = 1;

#ifdef DEBUG
	if (false) {
		std::cout << "Enter start value: ";
		std::cin >> startValue;
		std::cout << "Enter end value: ";
		std::cin >> endValue;
		if (startValue > MAXIMUM_VALUE || endValue > MAXIMUM_VALUE) {
			if (startValue > endValue) {
				std::cout << "ERROR: Maximum value 68,719,476,735 is exceeded.";
			}
		} else {
			std::cout << "WARNING: Start value is greater than end value. They will be inverted.";
			std::cout << std::endl;
			temporary = startValue;
			startValue = endValue;
			endValue = temporary;
		}
	} else {
		std::cout << "Value: ";
		values = normalImages(1);
		std::cout << values[0] << std::endl;
	}

	imageValues = generateImageValues(generateArray(values[0]));
	for (i = 0; i < APRIL_TAG_WIDTH; i++) {
		for (j = 0; j < APRIL_TAG_WIDTH; j++) {
			pictureValues[i][j] = imageValues[i][j];
		}
	}
	cv::Mat aprilTag(APRIL_TAG_WIDTH, APRIL_TAG_WIDTH, CV_64F, pictureValues);

	for (i = 0; i < APRIL_TAG_WIDTH; i++) {
		for (j = 0; j < APRIL_TAG_WIDTH; j++) {
			std::cout << aprilTag.at<double>(i, j) << "\t";
		}
		std::cout << std::endl;
	}
#endif // DEBUG

	do {
		std::cout << std::endl;
		displayOptions(startValue, endValue, fileName, mean, standardDeviation, numberOfImages);
		displayMenu();
		std::cin >> choice;
		switch (choice) {
		case 0:
			break;
		case 1:
			unsigned long temporary;
			for (temporary = startValue; temporary < endValue; temporary++) {
				imageValues = generateImageValues(generateArray(temporary));
				for (j = 0; j < APRIL_TAG_WIDTH; j++) {
					for (k = 0; k < APRIL_TAG_WIDTH; k++) {
						pictureValues[j][k] = imageValues[j][k];
					}
				}
				cv::Mat aprilTag(APRIL_TAG_WIDTH, APRIL_TAG_WIDTH, CV_64F, pictureValues);
				saveImage(fileName + "_" + std::to_string(i) + ".png", aprilTag);
				cv::imshow("AprilTag", aprilTag);
				cv::waitKey(3);
			}
			break;
		case 2:
			std::cout << "Enter start value: ";
			std::cin >> startValue;
			break;
		case 3:
			std::cout << "Enter end value: ";
			std::cin >> endValue;
			break;
		case 4:
			std::cout << "Enter filename: ";
			std::cin >> fileName;
			break;
		case 5:
			break;
		case 6:
			values = normalImages(numberOfImages, mean, standardDeviation);
			for (i = 0; i < values.size(); i++) {
				imageValues = generateImageValues(generateArray(values[i]));
				for (j = 0; j < APRIL_TAG_WIDTH; j++) {
					for (k = 0; k < APRIL_TAG_WIDTH; k++) {
						pictureValues[j][k] = imageValues[j][k];
					}
				}
				cv::Mat aprilTag(APRIL_TAG_WIDTH, APRIL_TAG_WIDTH, CV_64F, pictureValues);
				saveImage(fileName + "_" + std::to_string(i) + ".png", aprilTag);
				cv::imshow("AprilTag", aprilTag);
				cv::waitKey(3);
			}
			break;
		case 7:
			std::cout << "Enter mean: ";
			std::cin >> mean;
			break;
		case 8:
			std::cout << "Enter standard deviation: ";
			std::cin >> standardDeviation;
			break;
		case 9:
			std::cout << "Enter number of images: ";
			std::cin >> numberOfImages;
			break;
		case 10:
			printHelp();
			break;
		default:
			std::cout << "Invalid choice!" << std::endl;
			break;
		}
	} while (choice);



	//system("pause");
	cv::destroyWindow("AprilTag");
	return 0;
}

void displayMenu() {
	std::cout << "0. Exit" << std::endl;
	std::cout << "1. Generate sequential images" << std::endl;
	std::cout << "2. Change start value" << std::endl;
	std::cout << "3. Change end value" << std::endl;
	std::cout << "4. Change filename" << std::endl;
	std::cout << "6. Generate normal images" << std::endl;
	std::cout << "7. Change mean" << std::endl;
	std::cout << "8. Change deviation" << std::endl;
	std::cout << "9. Change number of images" << std::endl;
	std::cout << "10. Help" << std::endl;
	std::cout << "Enter choice: ";
}

void displayOptions(int startValue, int endValue, std::string filename) {
	std::cout << "Start value:\t" << startValue << std::endl;
	std::cout << "End value:\t" << endValue << std::endl;
	std::cout << "Filename:\t" << filename << std::endl;
}

void displayOptions(int startValue, int endValue, std::string filename, double mean, double standardDeviation, unsigned long numberOfImages) {
	std::cout << "Start value:\t" << startValue << std::endl;
	std::cout << "End value:\t" << endValue << std::endl;
	std::cout << "Filename:\t" << filename << std::endl;
	std::cout << "Mean:\t" << mean << std::endl;
	std::cout << "Standard deviation:\t" << standardDeviation << std::endl;
	std::cout << "Number of images:\t" << numberOfImages << std::endl;
}

bool saveImage(std::string filename, cv::Mat input) {
	bool output;
	cv::Mat temporary;
	double ratio;
	double counter;
	const int MAXIMUM_PIXEL_STRENGTH = 255;
	const int HALF_MAXIMUM_PIXEL_STRENGTH = 124;

	ratio = static_cast<double>(SD_WIDTH) / static_cast<double>(APRIL_TAG_WIDTH);
	input.convertTo(input, CV_8U, 255.0);
	for (counter = static_cast<double>(APRIL_TAG_WIDTH); counter < static_cast<double>(SD_WIDTH); counter = counter * 2) {
		cv::resize(input, temporary, cv::Size(), 2.0, 2.0);
		cv::resize(temporary, input, cv::Size(), 1.0, 1.0);
		cv::threshold(input, input, HALF_MAXIMUM_PIXEL_STRENGTH, MAXIMUM_PIXEL_STRENGTH, cv::THRESH_BINARY);
	}
	output = cv::imwrite(filename, temporary);

	return output;
}

std::vector<std::vector<bool>> generateArray(unsigned long input) {
	std::vector<std::vector<bool>> output(APRIL_TAG_PAYLOAD_WIDTH, std::vector<bool>(APRIL_TAG_PAYLOAD_WIDTH));
	int i, j;
	unsigned long temporary;
	unsigned long divisor;
	const unsigned long BASE_NUMBER = 2;

	divisor = BASE_NUMBER;
	temporary = input;
	for (int i = 0; i < APRIL_TAG_PAYLOAD_WIDTH; i++) {
		for (int j = 0; j < APRIL_TAG_PAYLOAD_WIDTH; j++) {
			if (temporary % divisor > 0) {
				output[i][j] = false;
			} else {
				output[i][j] = true;
			}
			temporary = temporary / divisor;
		}
	}

	return output;
}

cv::Mat generateImage(std::vector<std::vector<bool>> input) {
	int i, j;
	double aprilTag[APRIL_TAG_WIDTH][APRIL_TAG_WIDTH];
	bool value[APRIL_TAG_PAYLOAD_WIDTH * APRIL_TAG_PAYLOAD_WIDTH];
	bool onBorder;

	assert(input.size() == APRIL_TAG_PAYLOAD_WIDTH);
	for (i = 0; i < input.size(); i++) {
		assert(input[i].size() == APRIL_TAG_PAYLOAD_WIDTH);
	}

	for (i = 0; i < APRIL_TAG_WIDTH; i++) {
		for (j = 0; j < APRIL_TAG_WIDTH; j++) {
			onBorder = false;
			aprilTag[i][j] = 1.0;
			if (i == 1 && j != 0 && j != APRIL_TAG_WIDTH - 1) {
				aprilTag[i][j] = 0;
				onBorder = true;
			} else if (i == (APRIL_TAG_WIDTH - 1) - 1 && j != 0 && j != APRIL_TAG_WIDTH - 1) {
				aprilTag[i][j] = 0;
				onBorder = true;
			} else if (j == 1 && i != 0 && i != APRIL_TAG_WIDTH - 1) {
				aprilTag[i][j] = 0;
				onBorder = true;
			} else if (j == (APRIL_TAG_WIDTH - 1) - 1 && i != 0 && i != APRIL_TAG_WIDTH - 1) {
				aprilTag[i][j] = 0;
				onBorder = true;
			}

			if (i == 0 || i == APRIL_TAG_WIDTH - 1) {
				onBorder = true;
			} else if (j == 0 || j == APRIL_TAG_WIDTH - 1) {
				onBorder = true;
			}

			if (!onBorder) {
				if (input[i - 2][j - 2]) {
					aprilTag[i][j] = 1.0;
				} else {
					aprilTag[i][j] = 0.0;
				}

			}
		}
	}
	cv::Mat output(APRIL_TAG_WIDTH, APRIL_TAG_WIDTH, CV_64F, aprilTag);

#ifdef DEBUG
	std::cout << "Numeric Array" << std::endl;
	for (i = 0; i < APRIL_TAG_WIDTH; i++) {
		for (j = 0; j < APRIL_TAG_WIDTH; j++) {
			std::cout << aprilTag[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "Image Array" << std::endl;
	for (i = 0; i < APRIL_TAG_WIDTH; i++) {
		for (j = 0; j < APRIL_TAG_WIDTH; j++) {
			std::cout << output.at<double>(i, j) << "\t";
		}
		std::cout << std::endl;
	}
#endif // DEBUG

	return output;
}

void generateImage(std::vector<std::vector<bool>> input, cv::Mat& output) {
	int i, j;
	double aprilTag[APRIL_TAG_WIDTH][APRIL_TAG_WIDTH];
	bool value[APRIL_TAG_PAYLOAD_WIDTH * APRIL_TAG_PAYLOAD_WIDTH];
	bool onBorder;

	assert(input.size() == APRIL_TAG_PAYLOAD_WIDTH);
	for (i = 0; i < input.size(); i++) {
		assert(input[i].size() == APRIL_TAG_PAYLOAD_WIDTH);
	}

	for (i = 0; i < APRIL_TAG_WIDTH; i++) {
		for (j = 0; j < APRIL_TAG_WIDTH; j++) {
			onBorder = false;
			aprilTag[i][j] = 1.0;
			if (i == 1 && j != 0 && j != APRIL_TAG_WIDTH - 1) {
				aprilTag[i][j] = 0;
				onBorder = true;
			} else if (i == (APRIL_TAG_WIDTH - 1) - 1 && j != 0 && j != APRIL_TAG_WIDTH - 1) {
				aprilTag[i][j] = 0;
				onBorder = true;
			} else if (j == 1 && i != 0 && i != APRIL_TAG_WIDTH - 1) {
				aprilTag[i][j] = 0;
				onBorder = true;
			} else if (j == (APRIL_TAG_WIDTH - 1) - 1 && i != 0 && i != APRIL_TAG_WIDTH - 1) {
				aprilTag[i][j] = 0;
				onBorder = true;
			}

			if (i == 0 || i == APRIL_TAG_WIDTH - 1) {
				onBorder = true;
			} else if (j == 0 || j == APRIL_TAG_WIDTH - 1) {
				onBorder = true;
			}

			if (!onBorder) {
				aprilTag[i][j] = input[i - 2][j - 2];
			}
		}
	}
	output = cv::Mat(APRIL_TAG_WIDTH, APRIL_TAG_WIDTH, CV_64F, aprilTag);
}

std::vector<std::vector<double>> generateImageValues(std::vector<std::vector<bool>> input) {
	int i, j;
	std::vector<std::vector<double>> output(APRIL_TAG_WIDTH, std::vector<double>(APRIL_TAG_WIDTH));
	bool onBorder;

	assert(input.size() == APRIL_TAG_PAYLOAD_WIDTH);
	for (i = 0; i < input.size(); i++) {
		assert(input[i].size() == APRIL_TAG_PAYLOAD_WIDTH);
	}

	for (i = 0; i < APRIL_TAG_WIDTH; i++) {
		for (j = 0; j < APRIL_TAG_WIDTH; j++) {
			onBorder = false;
			output[i][j] = 1.0;
			if (i == 1 && j != 0 && j != APRIL_TAG_WIDTH - 1) {
				output[i][j] = 0;
				onBorder = true;
			} else if (i == (APRIL_TAG_WIDTH - 1) - 1 && j != 0 && j != APRIL_TAG_WIDTH - 1) {
				output[i][j] = 0;
				onBorder = true;
			} else if (j == 1 && i != 0 && i != APRIL_TAG_WIDTH - 1) {
				output[i][j] = 0;
				onBorder = true;
			} else if (j == (APRIL_TAG_WIDTH - 1) - 1 && i != 0 && i != APRIL_TAG_WIDTH - 1) {
				output[i][j] = 0;
				onBorder = true;
			}

			if (i == 0 || i == APRIL_TAG_WIDTH - 1) {
				onBorder = true;
			} else if (j == 0 || j == APRIL_TAG_WIDTH - 1) {
				onBorder = true;
			}

			if (!onBorder) {
				output[i][j] = input[i - 2][j - 2];
			}
		}
	}
	
	return output;
}

// Generate a normal distribution of AprilTag images
std::vector<unsigned long> normalImages(unsigned long number) {
	const double STANDARD_MEAN = MAXIMUM_VALUE / 2.0;
	const double STANDARD_DEVIATION = MAXIMUM_VALUE / 8.0;
	
	return normalImages(number, STANDARD_MEAN, STANDARD_DEVIATION);
}

// Generate a normal distribution of AprilTag images
std::vector<unsigned long> normalImages(unsigned long number, double mean, double standardDeviation) {
	std::vector<unsigned long> output;
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(mean, standardDeviation);

	generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
	for (unsigned long i = 0; i < number; i++) {
		unsigned long j = distribution(generator);
		output.push_back(j);
	}

	return output;
}

void printHelp() {
	std::cout << "(0) Exit - Exit application" << std::endl;
	std::cout << "(1) Generate sequential images - Create images from start value to end value" << std::endl;
	std::cout << "(2) Change start value - Change start value" << std::endl;
	std::cout << "(3) Change end value - Change end value" << std::endl;
	std::cout << "(4) Change filename - Change filename under which images are stored" << std::endl;
	std::cout << "(6) Generate normal images - Create images with a random value of normal distribution" << std::endl;
	std::cout << "(7) Change mean - Change mean which is is used for generating normal image distribution" << std::endl;
	std::cout << "(8) Change deviation - Change standard deviation of normal image distribution" << std::endl;
	std::cout << "(9) Change number of images - Change the number of images generated with (6)" << std::endl;
	std::cout << "(10) Help - Display information on selections" << std::endl;
}