#!/bin/sh

# Directory to store downloaded .deb files
output_dir="debs_offline"

# Mounted directory where you want to extract the packages
mount_dir="/mnt"

# List of packages to download (arguments to the script)
packages="$@"

# Ensure the mount directory exists
if [ ! -d "$mount_dir" ]; then
  echo "Error: Mount directory $mount_dir does not exist."
  exit 1
fi

# Create the output directory to store the downloaded .deb files
mkdir -pv "$output_dir"
cd "$output_dir"
# Function to get dependencies recursively using apt-cache
get_dependencies() {
    pkg=$1
    apt-cache depends $pkg | grep "Depends:" | awk '{print $2}'
}

# Loop through each package and resolve dependencies manually
for pkg in $packages; do
    echo "Resolving dependencies for $pkg..."

    # Download the main package
    echo "Downloading $pkg..."
    apt-get download $pkg -o=dir::cache="$output_dir" || {
        echo "Failed to download $pkg. Check for network issues or package availability."
        exit 1
    }

    # Resolve and download dependencies
    deps=$(get_dependencies $pkg)
    for dep in $deps; do
        echo "Downloading dependency $dep..."
        apt-get download $dep -o=dir::cache="$output_dir" || {
            echo "Failed to download $dep."
        }

        # Recursively download dependencies of dependencies
        subdeps=$(get_dependencies $dep)
        for subdep in $subdeps; do
            echo "Downloading sub-dependency $subdep..."
            apt-get download $subdep -o=dir::cache="$output_dir" || {
                echo "Failed to download $subdep."
            }
        done
    done
done

Extract all the downloaded .deb packages to the mounted directory
echo "Extracting packages to $mount_dir..."
for deb in "$output_dir"/*.deb; do
    if [ -e "$deb" ]; then
        echo "Extracting $deb..."
        dpkg-deb -x "$deb" "$mount_dir" || {
            echo "Failed to extract $deb"
            exit 1
        }
    else
        echo "No .deb files found in $output_dir."
        break
    fi
done

# echo "All packages have been downloaded and extracted to $mount_dir."
