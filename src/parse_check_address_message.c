#include "parse_check_address_message.h"
#include "globals.h"
//                                                      offset
// 1 byte (length X of "to" currency configuration)     0
// X bytes ("to" currency configuration)                1
// D bytes = 2 + C (DER serialized signature):          1 + X
//    1 byte 0x30                                       1 + X
//    1 byte length C of compoind object                2 + X
//    C bytes
// 1 byte length Y of address paramaters                1 + X + D
// Y bytes of address parameters                        2 + X + D
int parse_check_address_message(const command_t *cmd,
                                buf_t *config,
                                buf_t *der,
                                buf_t *address_parameters) {
    if (cmd->data.size < MIN_DER_SIGNATURE_LENGTH + 2) {
        return 0;
    }

    config->size = cmd->data.bytes[0];
    config->bytes = cmd->data.bytes + 1;

    if (cmd->data.size < 2 + MIN_DER_SIGNATURE_LENGTH + config->size) {
        return 0;
    }

    der->size = cmd->data.bytes[1 + config->size + 1] + 2;
    der->bytes = cmd->data.bytes + 1 + config->size;

    if (der->size < MIN_DER_SIGNATURE_LENGTH ||  //
        der->size > MAX_DER_SIGNATURE_LENGTH ||  //
        cmd->data.size < 2 + der->size + config->size) {
        return 0;
    }

    address_parameters->size = cmd->data.bytes[1 + config->size + der->size];
    address_parameters->bytes = cmd->data.bytes + 1 + config->size + der->size + 1;

    if (cmd->data.size != 1 + config->size + der->size + 1 + address_parameters->size) {
        return 0;
    }

    return 1;
}
