#!/bin/bash

# Base directory
BASE_DIR="2025"

# Iterate over each item in the 2025 directory
for problem_dir in "$BASE_DIR"/*; do
    # Check if it's a directory
    if [ -d "$problem_dir" ]; then
        # Get the name of the folder (e.g., A, B, C...)
        folder_name=$(basename "$problem_dir")
        
        # Path to the description directory
        desc_path="$problem_dir/description"
        
        # Check if description directory exists
        if [ -d "$desc_path" ]; then
            echo "Processing $folder_name..."
            
            # Create the .en.md file if it doesn't exist
            en_file="$desc_path/$folder_name.en.md"
            if [ ! -f "$en_file" ]; then
                touch "$en_file"
                echo "  Created $folder_name.en.md"
            else
                echo "  $folder_name.en.md already exists"
            fi
            
            # Create the .pt.md file if it doesn't exist
            pt_file="$desc_path/$folder_name.pt.md"
            if [ ! -f "$pt_file" ]; then
                touch "$pt_file"
                echo "  Created $folder_name.pt.md"
            else
                echo "  $folder_name.pt.md already exists"
            fi
        else
            echo "Skipping $folder_name: description folder not found."
        fi
    fi
done

echo "Done!"
