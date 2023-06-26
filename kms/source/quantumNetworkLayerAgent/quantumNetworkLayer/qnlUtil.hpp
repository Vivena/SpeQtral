#include <iostream>
#include <fstream>
#include <cstring>

/**
 * @brief A utility class that provides helpful functionality.
 */
class qnlUtil {
public:


    /**
     * @brief Perform some action with no arguments and no return value.
     */
    void doSomething();

    /**
     * @brief Write the contents of a byte array to a file in blocks of a specified size.
     *
     * @param dst The byte array to write.
     * @param file The filename to write the data to.
     * @param keyBlockSz The block size to use when writing the data.
     * @return true if the file was successfully written to, false otherwise.
     */
    bool writeFileBlocks(const char* dst, const char* file, int keyBlockSz){
    // Open the file for writing in binary mode
    std::ofstream outfile(file, std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file: " << file << std::endl;
        return false;
    }

    // Write the data to the file in blocks of keyBlockSz bytes
    int offset = 0;
    while (offset < strlen(dst)) {
        // Determine the number of bytes to write in this block
        int numBytes = strlen(dst) - offset;
        if (numBytes > keyBlockSz) {
            numBytes = keyBlockSz;
        }
        
        // Write the current block to the file
        outfile.write(&dst[offset], numBytes);

        // Add newline at end of block
        outfile.put('\n');
        
        // Update the offset into the byte array
        offset += numBytes;
    }

    // Close the file
    outfile.close();

    return true;
}

};
