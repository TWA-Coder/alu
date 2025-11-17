#!/bin/bash

# System Monitoring Shell Script

# Log file location
LOG_FILE="system_monitor.log"

# Default thresholds
CPU_THRESHOLD=80
MEMORY_THRESHOLD=80
DISK_THRESHOLD=80

# Function to log messages with timestamps
log_message() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" >> "$LOG_FILE"
}

# Function to display system status
check_system_status() {
    echo "----- System Status -----"
    echo "CPU Usage: $(top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{print 100 - $1}')%"
    echo "Memory Usage: $(free | grep Mem | awk '{printf("%.2f%%\n", ($3/$2)*100)}')"
    echo "Disk Usage: $(df -h | grep '^/dev' | awk '{print $1 ": " $5}')"
    echo "-------------------------"
}

# Function to check thresholds and log alerts
check_thresholds() {
    CPU_USAGE=$(top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{print 100 - $1}')
    MEMORY_USAGE=$(free | grep Mem | awk '{printf("%.0f\n", ($3/$2)*100)}')
    DISK_USAGE=$(df | grep '^/dev' | awk '{print $5}' | sed 's/%//g')

    if (( $(echo "$CPU_USAGE > $CPU_THRESHOLD" | bc -l) )); then
        log_message "WARNING: CPU usage exceeded threshold: ${CPU_USAGE}%"
    fi

    if (( MEMORY_USAGE > MEMORY_THRESHOLD )); then
        log_message "WARNING: Memory usage exceeded threshold: ${MEMORY_USAGE}%"
    fi

    if (( DISK_USAGE > DISK_THRESHOLD )); then
        log_message "WARNING: Disk usage exceeded threshold: ${DISK_USAGE}%"
    fi
}

# Function to display the menu
display_menu() {
    echo "----- System Monitoring Menu -----"
    echo "1. View System Status"
    echo "2. Set Alert Thresholds"
    echo "3. View Logs"
    echo "4. Clear Logs"
    echo "5. Exit"
    echo "----------------------------------"
}

# Function to set alert thresholds
set_thresholds() {
    read -p "Enter CPU usage threshold (default: $CPU_THRESHOLD): " new_cpu
    read -p "Enter Memory usage threshold (default: $MEMORY_THRESHOLD): " new_memory
    read -p "Enter Disk usage threshold (default: $DISK_THRESHOLD): " new_disk

    # Validate and set new thresholds
    [ -n "$new_cpu" ] && CPU_THRESHOLD=$new_cpu
    [ -n "$new_memory" ] && MEMORY_THRESHOLD=$new_memory
    [ -n "$new_disk" ] && DISK_THRESHOLD=$new_disk

    log_message "Thresholds updated: CPU=$CPU_THRESHOLD%, Memory=$MEMORY_THRESHOLD%, Disk=$DISK_THRESHOLD%"
}

# Function to view logs
view_logs() {
    if [ -e "$LOG_FILE" ]; then
        echo "----- Log File -----"
        cat "$LOG_FILE"
        echo "--------------------"
    else
        echo "Log file does not exist."
    fi
}

# Function to clear logs
clear_logs() {
    > "$LOG_FILE"
    echo "Logs cleared."
}

# Main execution loop
while true; do
    check_thresholds
    display_menu
    read -p "Select an option: " option

    case $option in
        1)
            check_system_status
            ;;
        2)
            set_thresholds
            ;;
        3)
            view_logs
            ;;
        4)
            clear_logs
            ;;
        5)
            echo "Exiting..."
            exit 0
            ;;
        *)
            echo "Invalid option. Please try again."
            ;;
    esac

    # Sleep for a specified interval (60 seconds)
    sleep 60
done