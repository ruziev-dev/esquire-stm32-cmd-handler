#include "esquire_message_handler.h"
#include "isotp_service.h"
#include "FS.h"
#include "FS_Tree.h"
#include "main.h"
#include "stm32f4xx_hal_cortex.h"
#include "FileTransferState.h"

uint8_t buff[1024];

bool esquire_cmd_handler(esquire_EcosystemCommand *cmd)
{
    switch (cmd->which_msg)
    {
    case esquire_EcosystemCommand_reset_cmd_tag:
        NVIC_SystemReset();
        break;
    case esquire_EcosystemCommand_send_fs_tree_tag:
    {
        uint16_t bytes_written;
        FS_Tree_getData(buff, 1024, &bytes_written);
        sendIsoTpCmdChannel(buff, bytes_written);
        break;
    }
    case esquire_EcosystemCommand_recieve_new_file_tag:
    {

        esquire_RecieveNewFile recieve_new_file = cmd->msg.recieve_new_file;

        if (file_transfer_state.is_process_runned)
        {
            DBG("[E] Recieving is already started");
            return false;
        }

        file_transfer_state.is_process_runned = true;
        strcpy(file_transfer_state.file_name, recieve_new_file.name.arg);
        strcpy(file_transfer_state.folder_path, recieve_new_file.folder_path.arg);
        memcpy(file_transfer_state.md5, recieve_new_file.md5.arg, sizeof(file_transfer_state.md5));
        file_transfer_state.size = recieve_new_file.size;

        int ret = FS_FileRecordingInit(file_transfer_state.file_name, file_transfer_state.folder_path);
        DBG("Recieve new file size: %ub. path: %s name: %s initState: %i", file_transfer_state.size, (char *)file_transfer_state.folder_path, (char *)file_transfer_state.file_name, ret);

        uint8_t *md5Arr = (uint8_t *)file_transfer_state.md5;

        DBG("md5: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
            md5Arr[0],
            md5Arr[1],
            md5Arr[2],
            md5Arr[3],
            md5Arr[4],
            md5Arr[5],
            md5Arr[6],
            md5Arr[7],
            md5Arr[8],
            md5Arr[9],
            md5Arr[10],
            md5Arr[11],
            md5Arr[12],
            md5Arr[13],
            md5Arr[14],
            md5Arr[15]

        );

        break;
    }
    case esquire_EcosystemCommand_recieve_block_tag:
    {
        if (!file_transfer_state.is_process_runned)
        {
            DBG("[E] is_process_runned=false");
            return false;
        }

        esquire_RecieveBlock recieve_block = cmd->msg.recieve_block;

        file_transfer_state.new_chunk.chunk_num = recieve_block.number;
        file_transfer_state.new_chunk.buffer_size = recieve_block.size;

        DBG("recieve_block #%lu with size: %u", file_transfer_state.new_chunk.chunk_num, file_transfer_state.new_chunk.buffer_size);
        // TODO
        break;
    }
    case esquire_EcosystemCommand_save_file_cmd_tag:
    {
        // esquire_SaveFileCmd save_file_cmd = cmd->msg.save_file_cmd;
        FS_FileRecordingFinish();
        bool isFileCRC_OK = FS_FileMD5HashCheckApproved(file_transfer_state.file_name, file_transfer_state.md5);
        DBG("CRC: %s", isFileCRC_OK ? "OK" : "FAIL");

        file_transfer_state.is_process_runned = false;
        file_transfer_state.size = 0;
        memset(file_transfer_state.file_name, 0, sizeof(file_transfer_state.file_name));
        memset(file_transfer_state.md5, 0, sizeof(file_transfer_state.md5));

        DBG("save_file_cmd");
        // TODO
        break;
    }
    case esquire_EcosystemCommand_format_flash_tag:
    {
#ifdef DEBUG
        FS_FormatFlash();
        DBG("FS_FormatFlash");
        return true;
#endif
        return false;
    }

    case esquire_EcosystemCommand_remove_item_tag:
    {
        esquire_FsRemoveItemCmd remove_item = cmd->msg.remove_item;
        FS_Remove(remove_item.path.arg);
        break;
    }

    default:
        return false;
    }
    return true;
}
