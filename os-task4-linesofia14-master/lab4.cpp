#include <iostream>
#include <vector>
#include <cstdlib> 
#include "lab4.h"

using namespace std;

const int PAGE_TABLE_SIZE = 14;

// Inverted Page Table Structure
struct PageTableEntry {
    int vpn;       // Virtual Page Number
    bool r_bit;    // Reference Bit
    bool m_bit;    // Modified Bit
    unsigned char aging_counter; // 8-bit counter for Aging
};

// Function to initialize the page table
void initializePageTable(vector<PageTableEntry>& page_table) {
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        page_table[i].vpn = -1; // -1 indicates an empty frame
        page_table[i].r_bit = false;
        page_table[i].m_bit = false;
        page_table[i].aging_counter = 0;
    }
}

// Function to perform random page replacement
int randomReplacement(const vector<PageTableEntry>& page_table) {
    return uniform_rnd(0, PAGE_TABLE_SIZE - 1);
}

// Function to perform aging page replacement
int agingReplacement(const vector<PageTableEntry>& page_table) {
    unsigned char min_counter = 255; // Initialize to max value
    vector<int> candidates;

    // Find pages with the smallest counter value
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        if (page_table[i].aging_counter < min_counter) {
            min_counter = page_table[i].aging_counter;
            candidates.clear(); // New minimum found
            candidates.push_back(i);
        } else if (page_table[i].aging_counter == min_counter) {
            candidates.push_back(i); // Add to candidates
        }
    }

    // Randomly choose among the candidates
    if (candidates.size() > 1) {
        int rand_index = uniform_rnd(0, candidates.size() - 1);
        return candidates[rand_index];
    } else {
        return candidates[0];
    }
}

// Function to perform page replacement based on the selected algorithm
int pageReplacement(vector<PageTableEntry>& page_table, int algorithm) {
    if (algorithm == 1) {
        return randomReplacement(page_table);
    } else { // algorithm == 2
        return agingReplacement(page_table);
    }
}

// Function to simulate the timer interrupt
void timerInterrupt(vector<PageTableEntry>& page_table) {
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        // Right shift counter and add R bit to MSB
        page_table[i].aging_counter = (page_table[i].aging_counter >> 1) | (page_table[i].r_bit ? 0x80 : 0x00);
        page_table[i].r_bit = false; // Reset R bit
    }
}

// Function to print the page table state
void printPageTable(const vector<PageTableEntry>& page_table) {
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        if (page_table[i].vpn == -1) {
            cout << "#";
        } else {
            cout << page_table[i].vpn;
        }
        if (i < PAGE_TABLE_SIZE - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <algorithm_number>" << endl;
        return 1;
    }

    int algorithm = atoi(argv[1]); // 1 for Random, 2 for Aging

    if (algorithm != 1 && algorithm != 2) {
        cerr << "Invalid algorithm number. Choose 1 or 2." << endl;
        return 1;
    }

    vector<PageTableEntry> page_table(PAGE_TABLE_SIZE);
    initializePageTable(page_table);

    int instruction_count = 0;
    int access_type, virtual_page;

    while (cin >> access_type >> virtual_page) {
        instruction_count++;

        bool page_hit = false;
        int frame_index = -1;

        // Search for the page in the page table
        for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
            if (page_table[i].vpn == virtual_page) {
                page_hit = true;
                frame_index = i;
                break;
            }
        }

        if (page_hit) {
            // Page Hit: Update R and M bits
            page_table[frame_index].r_bit = true;
            if (access_type == 1) {
                page_table[frame_index].m_bit = true;
            }
        } else {
            // Page Fault
            bool empty_frame_found = false;

            // Check for empty frames
            for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
                if (page_table[i].vpn == -1) {
                    // Found an empty frame, load the page
                    page_table[i].vpn = virtual_page;
                    page_table[i].r_bit = true;
                    page_table[i].m_bit = (access_type == 1); // Set M bit on write
                    page_table[i].aging_counter = 0;  // Reset aging counter
                    empty_frame_found = true;
                    break;
                }
            }

            // If no empty frames, perform page replacement
            if (!empty_frame_found) {
                int replace_index = -1;

                // Use the pageReplacement function to select the page to replace
                replace_index = pageReplacement(page_table, algorithm);

                // Replace the page
                page_table[replace_index].vpn = virtual_page;
                page_table[replace_index].r_bit = true;
                page_table[replace_index].m_bit = (access_type == 1); // Set M bit on write
                page_table[replace_index].aging_counter = 0; // Reset aging counter
            }
        }

        // Timer Interrupt (every 5 instructions)
        if (instruction_count % 5 == 0 && algorithm == 2) {
            timerInterrupt(page_table);
        }

        printPageTable(page_table);
    }

    return 0;
}

