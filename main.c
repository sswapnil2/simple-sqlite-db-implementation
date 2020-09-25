#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum {
    META_COMMAND_SUCCESS,
    META_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
}  PrepareResult;

typedef enum {
    INSERT_STATEMENT,
    SELECT_STATEMENT
} StatementType;

typedef struct {
    StatementType type;
} Statement;


InputBuffer* new_input_buffer() {
    InputBuffer *inputBuffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    inputBuffer->buffer = NULL;
    inputBuffer->buffer_length = 0;
    inputBuffer->input_length = 0;

    return inputBuffer;
}

void print_prompt(){
    printf("db > ");
}

void read_input(InputBuffer *inputBuffer){
    ssize_t bytes_read = getline(&(inputBuffer->buffer), &(inputBuffer->buffer_length), stdin );
    if (bytes_read <= 0){
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    // ignore trailing new line
    inputBuffer->buffer_length = bytes_read - 1;
    inputBuffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *inputBuffer){
    free(inputBuffer->buffer);
    free(inputBuffer);
}


MetaCommandResult do_meta_command(InputBuffer* buffer){
    if (strcmp(buffer->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_UNRECOGNIZED_COMMAND;
    }
}


PrepareResult prepare_statement(InputBuffer* input_buffer,
                                Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = INSERT_STATEMENT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = SELECT_STATEMENT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement){

    switch (statement->type) {
        case (SELECT_STATEMENT):
            printf("Perform select operation for statement");
            break;

        case (INSERT_STATEMENT):
            printf("Perform insert operation for statement");
            break;
    }
}


int main(int argc, char* argv[]) {

    InputBuffer * inputBuffer = new_input_buffer();

    while (true){
        print_prompt();
        read_input(inputBuffer);
        if (inputBuffer->buffer[0] == '.'){
            switch (do_meta_command(inputBuffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command %s \n", inputBuffer->buffer);
                    continue;
            }

        }

        Statement statement;

        switch (prepare_statement(inputBuffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;

            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized command at the start of %s \n", inputBuffer->buffer);
                continue;
        }
        execute_statement(&statement);
        printf("Executed\n");
    }

    return 0;
}
