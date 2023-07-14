#include <iostream>
#include <fstream>

class FileBitReader {
private:
    std::ifstream inFile;

public:
    FileBitReader(const std::string& filename) {
        inFile.open(filename, std::ios::binary);
        if (!inFile.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
        }
    }

    ~FileBitReader() {
        if (inFile.is_open()) {
            inFile.close();
        }
    }

    bool readBits(int n, std::string& bits) {
        char byte;
        int bitCount = 0;

        while (bitCount < n && inFile.read(&byte, sizeof(char))) {
            for (int i = 7; i >= 0 && bitCount < n; i--) {
                // Extract each bit from the byte and append to the result string
                bool bit = (byte >> i) & 1;
                bits += (bit ? '1' : '0');
                bitCount++;
            }
        }

        return bitCount == n;
    }
};
