#include <iostream>
#include <vector>
#include <algorithm>

struct Block {
    int start;
    int size;
    bool free;
};

class MemoryManager {
private:
    std::vector<Block> blocks;
    int totalSize;

public:
    MemoryManager(int size) : totalSize(size) {
        blocks.push_back({0, size, true});  // Start with one large free block
    }

    int allocate(int size) {
        for (size_t i = 0; i < blocks.size(); ++i) {
            if (blocks[i].free && blocks[i].size >= size) {
                int originalSize = blocks[i].size;
                int originalStart = blocks[i].start;
                blocks[i].size = size;
                blocks[i].free = false;

                if (originalSize > size) {
                    blocks.insert(blocks.end(), {originalStart + size, originalSize - size, true});
                }

                return blocks[i].start;
            }
        }

        return -1;  // Not enough memory
    }

    void free(int start) {
        for (auto &block : blocks) {
            if (block.start == start && !block.free) {
                block.free = true;
                merge();
                return;
            }
        }
    }

    void merge() {
    if (blocks.empty()) return;  // If no blocks, return early

    // First, sort blocks by starting address to ensure they are in order
    std::sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b) {
        return a.start < b.start;
    });

    // Create a new vector to store merged blocks
    std::vector<Block> mergedBlocks;
    mergedBlocks.push_back(blocks[0]);  // Start with the first block

    // Iterate over the blocks and merge adjacent free blocks
    for (int i = 1; i < blocks.size(); ++i) {
        Block& lastBlock = mergedBlocks.back();
        const Block& currentBlock = blocks[i];
        
        // If the last block in the merged list is free and adjacent to the current block, merge them
        if (lastBlock.free && currentBlock.free && (lastBlock.start + lastBlock.size == currentBlock.start)) {
            lastBlock.size += currentBlock.size;  // Extend the size of the last block
        } else {
            mergedBlocks.push_back(currentBlock);  // Otherwise, just add the current block
        }
    }

    // Replace old block list with the new merged list
    blocks = std::move(mergedBlocks);
}


    void compact() {
        int freeSpace = 0;
        blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
            [&freeSpace](const Block& b) {
                if (b.free) freeSpace += b.size;
                return b.free;
            }), blocks.end());
        if (freeSpace > 0) {
            blocks.push_back({totalSize - freeSpace, freeSpace, true});
            std::sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b) {
                return a.start < b.start;
            });
        }
    }

    void report() {
        for (const auto& block : blocks) {
            std::cout << "Block start at: " << block.start << ", size: " << block.size
                      << ", " << (block.free ? "Free" : "Allocated") << std::endl;
        }
    }
};

int main() {
    MemoryManager manager(1024);  // Example with 1024 units of memory

    int loc1 = manager.allocate(100);
    int loc2 = manager.allocate(200);
    int loc3 = manager.allocate(500);

    manager.report();
    manager.free(loc1);
    manager.compact();
    manager.report();

    return 0;
}
