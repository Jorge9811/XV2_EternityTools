#include "VirtualStageEmb.h"
#include "Misc/IggyTexFile.h"

#define STAGE_IGGYTEX_PATH      "data/ui/iggy/STAGESELE.iggytex"
#define PORTRAITS_IGGYTEX_ID    5

// Original stages in their position in the iggytex
static const std::vector<std::string> original_stages =
{
    "",
    "BFpln",
    "BFrck",
    "BFrcn",
    "BFnmc",
    "BFnmb",
    "BFcel",
    "BFceb",
    "BFtwc",
    "BFtwn",
    "BFtwh",
    "BFkoh",
    "BFsky",
    "BFsin",
    "BFsei",
    "BFkai",
    "BFund",
    "BFspe",
    "BFspv",
    "BFice",
    "BFtfl",
    "BFgen",
    "BFten",
    "BFlnd",
    "DMtwh",
    "BFlnc",
    "BFhel",
    "BFtok",
    "BFtre",
    "BFsmt",
    "BFtwf",
    "BFpot"
};

#define WIDTH       240
#define HEIGHT      84
#define LINE_SIZE   384
#define ROW_SIZE    1536

EmbFile VirtualStageEmb::portraits, VirtualStageEmb::backgrounds;

static uint8_t *ExtractPortraitLossless(const uint8_t *buf, int xindex, int yindex, size_t *psize)
{
    static const uint8_t header[128] =
    {
        0x44, 0x44, 0x53, 0x20, 0x7C, 0x00, 0x00, 0x00, 0x07, 0x10, 0x08, 0x00, 0x54, 0x00, 0x00, 0x00,
        0xF0, 0x00, 0x00, 0x00, 0x60, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
        0x04, 0x00, 0x00, 0x00, 0x44, 0x58, 0x54, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    buf += 0x80;

    *psize = 0x80+((WIDTH*HEIGHT)/2);
    uint8_t *ret = new uint8_t[*psize];
    uint32_t full_y_size = LINE_SIZE*HEIGHT;

    int i = 0;

    for (int y = 0; y < HEIGHT/4; y++)
    {
        const uint8_t *line = buf + (yindex*full_y_size);
        line += (y*ROW_SIZE);
        line += xindex*WIDTH*2;

        memcpy(ret+0x80+i, line, WIDTH*2);
        i += (WIDTH*2);
    }

    memcpy(ret, header, sizeof(header));
    return ret;
}

bool VirtualStageEmb::Build()
{
    IggyTexFile it;

    if (!xv2fs->LoadFile(&it, STAGE_IGGYTEX_PATH))
    {
        DPRINTF("%s: Failed to load %s\n", FUNCNAME, STAGE_IGGYTEX_PATH);
        return false;
    }

    if (!game_stage02_emb)
    {
        if (!Xenoverse2::InitStageEmb())
            return false;
    }

    uint32_t idx = it.FindIndex(PORTRAITS_IGGYTEX_ID);
    if (idx == (uint32_t)-1)
    {
        DPRINTF("%s: %s is not like I expected.\n", FUNCNAME, STAGE_IGGYTEX_PATH);
        return false;
    }

    const IggyTexContainedFile &original_portraits = it[idx];

    for (const Xv2Stage &stage : *game_stage_def)
    {
        if (stage.ssid == 0xFF)
            continue;

        if (stage.ssid < XV2_ORIGINAL_NUM_SS_STAGES)
        {
            for (int i = 1; i < (int)original_stages.size(); i++)
            {
                if (stage.code == original_stages[i])
                {
                    int x = i % 3;
                    int y = i / 3;
                    size_t size;

                    uint8_t *buf = ExtractPortraitLossless(original_portraits.GetData(), x, y, &size);
                    portraits.AppendFile(buf, size, stage.code + ".dds", true);
                }
            }
        }
        else
        {
            const std::string dds_name = "ICO" + stage.code + ".dds";
            uint16_t idx = game_stage02_emb->FindIndexByName(dds_name);

            if (idx != (uint16_t)-1)
            {
                EmbContainedFile new_file = (*game_stage02_emb)[idx]; // Copy

                new_file.SetName(stage.code + ".dds");
                portraits.AppendFile(new_file);
            }
        }

        uint16_t idx = game_stage02_emb->FindIndexByName(stage.code + ".dds");
        if (idx != (uint16_t)-1)
        {
            backgrounds.AppendFile(*game_stage02_emb, idx);
        }
    }

    return true;
}
