#include "Cac2X2m.h"
#include "IniFile.h"

#define CAQR_PATH   "data/sound/VOX/Quest/TMQ/CAQR_ALL_VOX"
#define CAQR_EN_PATH   "data/sound/VOX/Quest/TMQ/en/CAQR_ALL_VOX"

static const std::vector<std::string> part_names =
{
    "Face_base",
    "Face_forehead",
    "Face_eye",
    "Face_nose",
    "Face_ear",
    "Hair",
    "Bust",
    "Pants",
    "Rist",
    "Boots"
};

static void convert_str_ref(std::string &output, const std::string &input, const std::string &new_entry_name)
{
    output = input;

    if (input.length() == 0)
        return;

    output = Utils::NormalizePath(output);

    std::vector<std::string> components;
    Utils::GetMultipleStrings(output, components, '/');

    if (components.size() == 0)
        return;

    std::string &file_part = components[components.size()-1];

    if (file_part.length() >= 3 && file_part[0] >= 'A' && file_part[0] <= 'Z' && file_part[1] >= 'A' &&
        file_part[1] <= 'Z' && file_part[2] >= 'A' && file_part[2] <= 'Z')
    {
        file_part = new_entry_name + file_part.substr(3);
    }

    output = file_part;
}

void add_file(const std::string &file, const std::string &base, std::unordered_set<std::string> &files, bool check)
{
    const std::string path = Utils::MakePathString(base, file);

    if (check && !xv2fs->FileExists(path))
        return;

    if (files.find(path) == files.end())
    {
        files.insert(path);
        printf("File added: %s\n", path.c_str());
    }
}

void add_file(const std::string &file, std::unordered_set<std::string> &files, bool check)
{
    if (check && !xv2fs->FileExists(file))
        return;

    if (files.find(file) == files.end())
    {
        files.insert(file);
        printf("File added: %s\n", file.c_str());
    }
}

bool Cac2X2m::BuildPart(BcsPart &output, const BcsPart &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name, const std::string &part_name)
{
    const std::string &old_entry_name = input.name;

    output = input;

    if (!input.valid)
        return true;

    strcpy(output.name, new_entry_name.c_str());

    add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model, 3) + "_" + part_name + ".emd", base_dir, files, true);
    add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model, 3) + "_" + part_name + ".emb", base_dir, files, true);
    add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model, 3) + "_" + part_name + ".emm", base_dir, files, true);
    add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model, 3) + "_" + part_name + ".dyt.emb", base_dir, files, true);
    add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model, 3) + "_" + part_name + ".emd", base_dir, files, true);
    add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model, 3) + "_" + part_name + "_LOD01.emd", base_dir, files, true);
    add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model, 3) + "_" + part_name + "_LOD02.emd", base_dir, files, true);
    add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model, 3) + "_" + part_name + "_LOD03.emd", base_dir, files, true);

    if (input.model2 != input.model && input.model2 != 0xFFFF)
    {
        add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model2, 3) + "_" + part_name + ".emd", base_dir, files, true);
        add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model2, 3) + "_" + part_name + ".emb", base_dir, files, true);
        add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model2, 3) + "_" + part_name + ".emm", base_dir, files, true);
        add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model2, 3) + "_" + part_name + ".dyt.emb", base_dir, files, true);
        add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model2, 3) + "_" + part_name + ".emd", base_dir, files, true);
        add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model2, 3) + "_" + part_name + "_LOD01.emd", base_dir, files, true);
        add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model2, 3) + "_" + part_name + "_LOD02.emd", base_dir, files, true);
        add_file(old_entry_name + "_" + Utils::ToStringAndPad(input.model2, 3) + "_" + part_name + "_LOD03.emd", base_dir, files, true);
    }

    for (int i = 0; i < 4; i++)
    {
        convert_str_ref(output.files[i], input.files[i], new_entry_name);

        if (input.files[i].length() > 0)
        {
            if (i == 0)
            {
                add_file(input.files[i] + ".emd", base_dir, files, true);
                add_file(input.files[i] + ".emb", base_dir, files, true);
                add_file(input.files[i] + ".emm", base_dir, files, true);
                add_file(input.files[i] + ".dyt.emb", base_dir, files, true);
                add_file(input.files[i] + "_LOD01.emd", base_dir, files, true);
                add_file(input.files[i] + "_LOD02.emd", base_dir, files, true);
                add_file(input.files[i] + "_LOD03.emd", base_dir, files, true);
            }
            else if (i == 1)
            {
                add_file(input.files[i] + ".emm", base_dir, files, false);
                add_file(input.files[i] + ".dyt.emb", base_dir, files, true);
            }
            else if (i == 2)
            {
                add_file(input.files[i] + ".emb", base_dir, files, false);
                add_file(input.files[i] + ".dyt.emb", base_dir, files, true);
            }
            else if (i == 3)
            {
                add_file(input.files[i] + ".ean", base_dir, files, false);
            }
        }
    }

    for (size_t i = 0; i < input.physics.size(); i++)
    {
        const BcsPhysics &inp_ph = input.physics[i];
        BcsPhysics &out_ph = output.physics[i];

        strcpy(out_ph.name, new_entry_name.c_str());

        for (int j = 0; j < 6; j++)
        {
            convert_str_ref(out_ph.unk_28[j], inp_ph.unk_28[j], new_entry_name);

            if (inp_ph.unk_28[j].length() > 0)
            {
                add_file(inp_ph.unk_28[j] + ".scd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".emd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".esk", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".emm", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".emb", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".dyt.emb", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".ean", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD01.emd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD01.esk", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD02.emd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD02.esk", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD03.emd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD03.esk", base_dir, files, true);
            }
        }
    }

    return true;
}

bool Cac2X2m::BuildFaceBase(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[0], input.parts[0], files, base_dir, new_entry_name, "Face_base");
}

bool Cac2X2m::BuildFaceForehead(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[1], input.parts[1], files, base_dir, new_entry_name, "Face_forehead");
}

bool Cac2X2m::BuildFaceEye(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[2], input.parts[2], files, base_dir, new_entry_name, "Face_eye");
}

bool Cac2X2m::BuildFaceNose(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[3], input.parts[3], files, base_dir, new_entry_name, "Face_nose");
}

bool Cac2X2m::BuildFaceEar(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[4], input.parts[4], files, base_dir, new_entry_name, "Face_ear");
}

bool Cac2X2m::BuildHair(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[5], input.parts[5], files, base_dir, new_entry_name, "Hair");
}

bool Cac2X2m::BuildBust(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[6], input.parts[6], files, base_dir, new_entry_name, "Bust");
}

bool Cac2X2m::BuildPants(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[7], input.parts[7], files, base_dir, new_entry_name, "Pants");
}

bool Cac2X2m::BuildRist(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[8], input.parts[8], files, base_dir, new_entry_name, "Rist");
}

bool Cac2X2m::BuildBoots(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (!input.valid)
        return true;

    output.valid = true;
    output.parts.resize(10);

    return BuildPart(output.parts[9], input.parts[9], files, base_dir, new_entry_name, "Boots");
}

bool Cac2X2m::MergePart(X2mFile *x2m, BcsPart &output, const BcsPart &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &part_name)
{
    const std::string &old_entry_name = input.name;
    const std::string &new_entry_name = x2m->GetEntryName();

    if (input.model != 0xFFFF)
    {
        if (!BuildPart(output, input, files, base_dir, new_entry_name, part_name))
            return false;
    }

    for (const BcsPhysics &inp_ph : input.physics)
    {
        BcsPhysics out_ph = inp_ph; // copy

        strcpy(out_ph.name, new_entry_name.c_str());

        for (int j = 0; j < 6; j++)
        {
            convert_str_ref(out_ph.unk_28[j], inp_ph.unk_28[j], new_entry_name);

            if (inp_ph.unk_28[j].length() > 0)
            {
                add_file(inp_ph.unk_28[j] + ".scd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".emd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".esk", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".emm", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".emb", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".dyt.emb", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + ".ean", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD01.emd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD01.esk", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD02.emd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD02.esk", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD03.emd", base_dir, files, true);
                add_file(inp_ph.unk_28[j] + "_LOD03.esk", base_dir, files, true);
            }
        }

        if (out_ph.unk_28[0].length() > 0 && !xv2fs->FileExists(Utils::MakePathString(base_dir, inp_ph.unk_28[0] + ".dyt.emb")))
        {
            size_t size;
            const std::string path = Utils::MakePathString(base_dir, old_entry_name + "_" + Utils::ToStringAndPad(output.model, 3) + "_" + part_name + ".dyt.emb");
            uint8_t *buf = xv2fs->ReadFile(path, &size);

            if (buf)
            {
                const std::string out = new_entry_name + "/" + out_ph.unk_28[0] + ".dyt.emb";

                bool ret = x2m->WriteFile(out, buf, size);
                delete[] buf;

                if (!ret)
                    return false;
            }
        }

        out_ph.unk_18 = 8;
        output.physics.push_back(out_ph);
    }

    return true;
}

bool Cac2X2m::MergeAccesory(X2mFile *x2m, BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, bool wig)
{
    if (!input.valid)
        return true;

    for (int i = 0; i < 10; i++)
    {
        const BcsPart &part_in = input.parts[i];
        BcsPart &part_output = output.parts[i];

        if (part_in.valid)
        {
            if (part_in.model == 0xFFFF || part_in.unk_18 == 4 || wig)
            {
                return MergePart(x2m, part_output, part_in, files, base_dir, part_names[i]);
            }
        }
    }

    return true;
}

bool Cac2X2m::BuildPartSet(X2mFile *x2m, BcsPartSet &output, const BcsFile &input, std::unordered_set<std::string> &files, const std::string &base_dir, uint32_t face_base, uint32_t face_forehead, uint32_t face_eye, uint32_t face_nose, uint32_t face_ear, uint32_t hair, uint32_t bust, uint32_t pants, uint32_t rist, uint32_t boots, uint32_t accesory)
{
    const std::vector<BcsPartSet> &input_sets = input.GetPartSets();
    const std::string &new_entry_name = x2m->GetEntryName();

    if (face_base < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[face_base];

        if (!BuildFaceBase(output, input_set, files, base_dir, new_entry_name))
            return false;
    }
    else
    {
        output.parts[0].valid = false;
    }

    if (face_forehead < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[face_forehead];

        if (!BuildFaceForehead(output, input_set, files, base_dir, new_entry_name))
            return false;
    }
    else
    {
        output.parts[1].valid = false;
    }

    if (face_eye < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[face_eye];

        if (!BuildFaceEye(output, input_set, files, base_dir, new_entry_name))
            return false;
    }
    else
    {
        output.parts[2].valid = false;
    }

    if (face_nose < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[face_nose];

        if (!BuildFaceNose(output, input_set, files, base_dir, new_entry_name))
            return false;
    }
    else
    {
        output.parts[3].valid = false;
    }

    if (face_ear < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[face_ear];

        if (!BuildFaceEar(output, input_set, files, base_dir, new_entry_name))
            return false;
    }
    else
    {
        output.parts[4].valid = false;
    }

    if (hair < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[hair];

        if (!BuildHair(output, input_set, files, base_dir, new_entry_name))
            return false;
    }
    else
    {
        output.parts[5].valid = false;
    }

    if (bust < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[bust];

        if (!BuildBust(output, input_set, files, base_dir, new_entry_name))
            return false;

        // Trendy bikini color fix
        if (bust == 461 && output.valid)
        {
            if (output.parts[PART_BUST].valid)
            {
                output.parts[PART_BUST].color_selectors.clear();
            }
        }
    }
    else
    {
        const BcsPartSet &input_set = input_sets[0];

        if (!BuildBust(output, input_set, files, base_dir, new_entry_name))
            return false;
    }

    if (pants < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[pants];

        if (!BuildPants(output, input_set, files, base_dir, new_entry_name))
            return false;

        // Trendy bikini color fix
        if (pants == 461 && output.valid)
        {
            if (output.parts[PART_PANTS].valid)
            {
                output.parts[PART_PANTS].color_selectors.clear();
            }
        }
    }
    else
    {
        const BcsPartSet &input_set = input_sets[0];

        if (!BuildPants(output, input_set, files, base_dir, new_entry_name))
            return false;
    }

    if (rist < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[rist];

        if (!BuildRist(output, input_set, files, base_dir, new_entry_name))
            return false;
    }
    else
    {
        const BcsPartSet &input_set = input_sets[0];

        if (!BuildRist(output, input_set, files, base_dir, new_entry_name))
            return false;
    }

    if (boots < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[boots];

        if (!BuildBoots(output, input_set, files, base_dir, new_entry_name))
            return false;
    }
    else
    {
        const BcsPartSet &input_set = input_sets[0];

        if (!BuildBoots(output, input_set, files, base_dir, new_entry_name))
            return false;
    }

    if (accesory < input_sets.size())
    {
        const BcsPartSet &input_set = input_sets[accesory];
        bool wig = (accesory >= 832 && accesory <= 834);

        if (!MergeAccesory(x2m, output, input_set, files, base_dir, wig))
            return false;
    }

    return true;
}

bool Cac2X2m::AddPartColor(BcsPart &part, const BcsFile &input, const std::string &part_name, uint32_t color)
{
    if (!part.valid)
        return true;

    uint32_t idx = input.FindPartColorsByName(part_name);
    if (idx == (uint32_t)-1)
        return true;

    const BcsPartColors &part_colors = input.GetPartsColors()[idx];
    if (color >= part_colors.colors.size())
        return true;

    BcsColorSelector sel;

    sel.part_colors = idx;
    sel.color = color;
    part.color_selectors.push_back(sel);

    return true;
}

bool Cac2X2m::SetHairColor(BcsPartSet &output, const BcsFile &input, uint32_t color)
{
    return AddPartColor(output.parts[5], input, "HAIR_", color);
}

bool Cac2X2m::SetSkinColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4)
{
    for (uint32_t i = 0; i < 10; i++)
    {
        if (i ==  2 || i == 5)
            continue;

        BcsPart &part = output.parts[i];

        if (!AddPartColor(part, input, "SKIN_", color1))
            return false;

        if (!AddPartColor(part, input, "SKIN_A_", color1))
            return false;

        if (!AddPartColor(part, input, "SKIN_B_", color2))
            return false;

        if (!AddPartColor(part, input, "SKIN_C_", color3))
            return false;

        if (!AddPartColor(part, input, "SKIN_D_", color4))
            return false;
    }

    return true;
}

bool Cac2X2m::SetEyesColor(BcsPartSet &output, const BcsFile &input, uint32_t color)
{
    return AddPartColor(output.parts[2], input, "eye_", color);
}

bool Cac2X2m::SetBustColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4)
{
    BcsPart &part = output.parts[6];

    if (!AddPartColor(part, input, "CC00_BUST_", color1))
        return false;

    if (!AddPartColor(part, input, "CC01_BUST_", color2))
        return false;

    if (!AddPartColor(part, input, "CC02_BUST_", color3))
        return false;

    if (!AddPartColor(part, input, "CC03_BUST_", color4))
        return false;

    return true;
}

bool Cac2X2m::SetPantsColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4)
{
    BcsPart &part = output.parts[7];

    if (!AddPartColor(part, input, "CC00_PANTS_", color1))
        return false;

    if (!AddPartColor(part, input, "CC01_PANTS_", color2))
        return false;

    if (!AddPartColor(part, input, "CC02_PANTS_", color3))
        return false;

    if (!AddPartColor(part, input, "CC03_PANTS_", color4))
        return false;

    return true;
}

bool Cac2X2m::SetRistColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4)
{
    BcsPart &part = output.parts[8];

    if (!AddPartColor(part, input, "CC00_RIST_", color1))
        return false;

    if (!AddPartColor(part, input, "CC01_RIST_", color2))
        return false;

    if (!AddPartColor(part, input, "CC02_RIST_" , color3))
        return false;

    if (!AddPartColor(part, input, "CC03_RIST_", color4))
        return false;

    return true;
}

bool Cac2X2m::SetBootsColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4)
{
    BcsPart &part = output.parts[9];

    if (!AddPartColor(part, input, "CC00_BOOTS_", color1))
        return false;

    if (!AddPartColor(part, input, "CC01_BOOTS_", color2))
        return false;

    if (!AddPartColor(part, input, "CC02_BOOTS_" , color3))
        return false;

    if (!AddPartColor(part, input, "CC03_BOOTS_", color4))
        return false;

    return true;
}

bool Cac2X2m::SetColors(BcsPartSet &output, const BcsFile &input, uint32_t skin_color1, uint32_t skin_color2, uint32_t skin_color3, uint32_t skin_color4, uint32_t hair_color, uint32_t eye_color, uint32_t bust_color1, uint32_t bust_color2, uint32_t bust_color3, uint32_t bust_color4, uint32_t pants_color1, uint32_t pants_color2, uint32_t pants_color3, uint32_t pants_color4, uint32_t rist_color1, uint32_t rist_color2, uint32_t rist_color3, uint32_t rist_color4, uint32_t boots_color1, uint32_t boots_color2, uint32_t boots_color3, uint32_t boots_color4, uint32_t paint_color1, uint32_t paint_color2, uint32_t paint_color3)
{
    if (!SetSkinColors(output, input, skin_color1, skin_color2, skin_color3, skin_color4))
        return false;

    if (!SetHairColor(output, input, hair_color))
        return false;

    if (!SetEyesColor(output, input, eye_color))
        return false;

    if (!SetBustColors(output, input, bust_color1, bust_color2, bust_color3, bust_color4))
        return false;

    if (!SetPantsColors(output, input, pants_color1, pants_color2, pants_color3, pants_color4))
        return false;

    if (!SetRistColors(output, input, rist_color1, rist_color2, rist_color3, rist_color4))
        return false;

    if (!SetBootsColors(output, input, boots_color1, boots_color2, boots_color3, boots_color4))
        return false;

    if (!SetPaintColors(output, input, paint_color1, paint_color2, paint_color3))
        return false;

    return true;
}

bool Cac2X2m::SetPaintColors(BcsPartSet &output, const BcsFile &input, uint32_t color1, uint32_t color2, uint32_t color3)
{
    for (int i = 0; i < 2; i++)
    {
        BcsPart &part = output.parts[i];

        if (!AddPartColor(part, input, "PAINT_A_", color1))
            return false;

        if (!AddPartColor(part, input, "PAINT_B_", color2))
            return false;

        if (!AddPartColor(part, input, "PAINT_C_", color3))
            return false;
    }

    return true;
}

bool Cac2X2m::SetBody(EskFile &output, EanFile &cam_output, const BcsFile &input, uint32_t body_shape)
{
    UNUSED(output);

    const std::vector<BcsBody> &bodies = input.GetBodies();

    if (body_shape >= bodies.size())
    {
        DPRINTF("%s: Body %d out of bounds.\n", FUNCNAME, body_shape);
        return false;
    }

    const BcsBody &body = bodies[body_shape];
    if (!body.valid)
        return false;

    bool found = false;
    float scale_x, scale_y, scale_z;

    for (const BcsBoneScale &bone : body.unk1s)
    {
        if (bone.name == "b_C_Base")
        {
            found = true;
            scale_x = bone.scale[0];
            scale_y = bone.scale[1];
            scale_z = bone.scale[2];
        }

        // NO MORE SKELETON MODIFICATION
        // Thanks to xv2patcher, the game will select the proper body from bcs, which is 100% reliable compared
        // to the horrible hack of the skeleton
        /*if (!output.ChangeBoneScale(bone.name, bone.unk_00[0], bone.unk_00[1], bone.unk_00[2]))
        {
            DPRINTF("%s: Failed to change bone scale of %s.\n", FUNCNAME, bone.name.c_str());
            return false;
        }*/
    }    

    if (found)
    {
        if (!cam_output.ScaleAnim(0, scale_x, scale_y, scale_z))
        {
            DPRINTF("%s: ScaleAnim failed.\n", FUNCNAME);
            return false;
        }
    }

    return true;
}

bool Cac2X2m::CopyPartSet(BcsPartSet &output, const BcsPartSet &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    output = input;

    if (!input.valid)
        return true;

    for (size_t i = 0; i < output.parts.size(); i++)
    {
        BcsPart &out_part = output.parts[i];
        const BcsPart &in_part = input.parts[i];

        if (in_part.valid)
        {
            if (!BuildPart(out_part, in_part, files, base_dir, new_entry_name, part_names[i]))
                return false;
        }
    }

    return true;
}


bool Cac2X2m::CopySSJ(BcsFile &output, const BcsFile &input, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    std::vector<BcsPartSet> &out_sets = output.GetPartSets();
    const std::vector<BcsPartSet> &in_sets = input.GetPartSets();

    if (out_sets.size() < 300 || in_sets.size() < 300)
        return false;

    const BcsPartSet &ssj1 = in_sets[297];
    const BcsPartSet &ssj2 = in_sets[298];
    const BcsPartSet &ssj3 = in_sets[299];

    if (!CopyPartSet(out_sets[297], ssj1, files, base_dir, new_entry_name))
        return false;

    if (!CopyPartSet(out_sets[298], ssj2, files, base_dir, new_entry_name))
        return false;

    if (!CopyPartSet(out_sets[299], ssj3, files, base_dir, new_entry_name))
        return false;

    if (!CopyPartSet(out_sets[100], ssj1, files, base_dir, new_entry_name))
        return false;

    if (!CopyPartSet(out_sets[101], ssj2, files, base_dir, new_entry_name))
        return false;

    if (!CopyPartSet(out_sets[102], ssj3, files, base_dir, new_entry_name))
        return false;

    if (!out_sets[0].valid)
        return true;

    for (int i = 100; i <= 299; i++)
    {
        if (i > 102 && i < 297)
            continue;        

        BcsPartSet &ssj = out_sets[i];

        if (!ssj.valid)
        {
            ssj.valid = true;
            ssj.parts.resize(10);
        }

        BcsPart &forehead = ssj.parts[1];
        BcsPart &eyes = ssj.parts[2];
        BcsPart &hair = ssj.parts[5];

        if (!forehead.valid)
        {
            forehead = out_sets[0].parts[1];
        }

        if (!eyes.valid)
        {
            eyes = out_sets[0].parts[2];

            for (size_t n = 0; eyes.valid && n < eyes.color_selectors.size(); n++)
            {
                const BcsColorSelector &cs = eyes.color_selectors[n];

                if (cs.part_colors == 5)
                {
                    eyes.color_selectors.erase(eyes.color_selectors.begin()+n);
                    n--;
                }
            }
        }

        if (!hair.valid)
        {
            hair = out_sets[0].parts[5];

            for (size_t n = 0; hair.valid && n < hair.color_selectors.size(); n++)
            {
                const BcsColorSelector &cs = hair.color_selectors[n];

                if (cs.part_colors == 5)
                {
                    hair.color_selectors.erase(hair.color_selectors.begin()+n);
                    n--;
                }
            }
        }
        else
        {
            if (hair.physics.size() == 0 && out_sets[0].parts[5].valid && out_sets[0].parts[5].physics.size() > 0)
            {
                hair.physics = out_sets[0].parts[5].physics;
            }
        }
    }

    return true;
}

bool Cac2X2m::CopyTransforms(BcsFile &output, const BcsFile &input, const std::vector<uint32_t> &trans_sets, const std::vector<uint32_t> &def_sets, uint32_t first_valid, std::unordered_set<std::string> &files, const std::string &base_dir, const std::string &new_entry_name)
{
    if (trans_sets.size() != def_sets.size())
        return false;

    if (trans_sets.size() == 0)
        return true;

    std::vector<BcsPartSet> &out_sets = output.GetPartSets();
    const std::vector<BcsPartSet> &in_sets = input.GetPartSets();

    if (out_sets.size() > 1 && !out_sets[0].valid)
        return true;

    for (size_t i = 0; i < trans_sets.size(); i++)
    {
        uint32_t ts = trans_sets[i];

        if (ts < first_valid || ts >= out_sets.size() || ts >= in_sets.size() || def_sets[i] >= out_sets.size())
            continue;

        BcsPartSet &out_set = out_sets[ts];
        const BcsPartSet &in_set = in_sets[ts];
        const BcsPartSet &def_set = out_sets[def_sets[i]];

        if (!CopyPartSet(out_set, in_set, files, base_dir, new_entry_name))
            return false;

        if (!def_set.valid)
            continue;

        if (!out_set.valid)
        {
            out_set.valid = true;
            out_set.parts.resize(10);
        }

        BcsPart &forehead = out_set.parts[1];
        BcsPart &eyes = out_set.parts[2];
        BcsPart &hair = out_set.parts[5];
        BcsPart &pants = out_set.parts[7];
        BcsPart &boots = out_set.parts[9];

        if (!forehead.valid)
        {
            forehead = def_set.parts[1];
        }

        if (!eyes.valid)
        {
            eyes = def_set.parts[2];

            for (size_t n = 0; eyes.valid && n < eyes.color_selectors.size(); n++)
            {
                const BcsColorSelector &cs = eyes.color_selectors[n];

                if (cs.part_colors == 5)
                {
                    eyes.color_selectors.erase(eyes.color_selectors.begin()+n);
                    n--;
                }
            }
        }

        if (!hair.valid)
        {
            hair = def_set.parts[5];

            for (size_t n = 0; hair.valid && n < hair.color_selectors.size(); n++)
            {
                const BcsColorSelector &cs = hair.color_selectors[n];

                if (cs.part_colors == 5)
                {
                    hair.color_selectors.erase(hair.color_selectors.begin()+n);
                    n--;
                }
            }
        }
        else
        {
            if (hair.physics.size() == 0 && def_set.parts[5].valid && def_set.parts[5].physics.size() > 0)
            {
                hair.physics = def_set.parts[5].physics;
            }
        }

        if (pants.valid && pants.color_selectors.size() == 0)
        {
            pants.color_selectors = def_set.parts[7].color_selectors;
        }

        if (boots.valid && boots.color_selectors.size() == 0)
        {
            boots.color_selectors = def_set.parts[9].color_selectors;
        }
    }

    return true;
}

bool FixSSJ3DytEmb(X2mFile *x2m, const std::string &emb_path)
{
    size_t size;
    uint8_t *buf = x2m->ReadFile(emb_path, &size);

    if (!buf)
        return false;

    EmbFile emb;

    bool ret = emb.Load(buf, size);
    delete[] buf;

    if (!ret)
        return false;

    size_t num = emb.GetNumFiles();
    if (num == 3)
    {
        while (emb.GetNumFiles() != 6)
        {
            emb.AppendFile(emb[2]);
        }

        buf = emb.Save(&size);
        if (!buf)
        {
            DPRINTF("%s: emb save failed.\n", FUNCNAME);
            return false;
        }

        ret = x2m->WriteFile(emb_path, buf, size);
        delete[] buf;

        if (!ret)
        {
            DPRINTF("%s: X2mFile::WriteFile failed.\n", FUNCNAME);
            return false;
        }
    }

    return true;
}

/*bool Cac2X2m::FixSSJ3(X2mFile *x2m, const BcsFile &new_bcs)
{
    const std::vector<BcsPartSet> &sets = new_bcs.GetPartSets();
    if (sets.size() < 300)
        return false;

    const BcsPartSet &ssj3 = sets[299];
    if (!ssj3.valid)
        return true;

    const BcsPart &hair = ssj3.parts[5];
    if (!hair.valid || hair.physics.size() < 0)
        return true;

    for (const BcsPhysics &ph : hair.physics)
    {
        const std::string &rel_path = ph.unk_28[0];

        if (rel_path.find("hair_scd") != std::string::npos)
        {
            const std::string path = x2m->GetEntryName() + "/" + rel_path + ".dyt.emb";

            size_t size;
            uint8_t *buf = x2m->ReadFile(path, &size);

            if (buf)
            {
                EmbFile emb;

                if (emb.Load(buf, size))
                {
                    size_t num = emb.GetNumFiles();
                    if (num >= 3 && num < 6)
                    {
                        while (emb.GetNumFiles() != 6)
                        {
                            emb.AppendFile(emb[2]);
                        }

                        buf = emb.Save(&size);
                        if (!buf)
                        {
                            DPRINTF("%s: emb save failed.\n", FUNCNAME);
                            return false;
                        }

                        if (!x2m->WriteFile(path, buf, size))
                        {
                            DPRINTF("%s: X2mFile::WriteFile failed.\n", FUNCNAME);
                            return false;
                        }
                    }
                }
            }

            break;
        }
    }

    return true;
}*/
bool fix_ssj3_dyt_visitor(const std::string &file, void *param)
{
    X2mFile *x2m = (X2mFile *)param;

    FixSSJ3DytEmb(x2m, file);
    return true;
}

bool Cac2X2m::FixSSJ3(X2mFile *x2m)
{

    x2m->VisitDirectory(x2m->GetEntryName(), fix_ssj3_dyt_visitor, x2m);
    return true;
}

uint32_t Cac2X2m::BustItem(uint32_t item_id)
{
    if (item_id == 0xFFFF || item_id == 0xFFFFFFFF)
        return (uint32_t)-1;

    IdbEntry *entry = game_top_idb->FindEntryByID(item_id);
    if (entry)
        return entry->model;

    return (uint32_t)-1;
}

uint32_t Cac2X2m::PantsItem(uint32_t item_id)
{
    if (item_id == 0xFFFF || item_id == 0xFFFFFFFF)
        return (uint32_t)-1;

    IdbEntry *entry = game_bottom_idb->FindEntryByID(item_id);
    if (entry)
        return entry->model;

    return (uint32_t)-1;
}

uint32_t Cac2X2m::RistItem(uint32_t item_id)
{
    if (item_id == 0xFFFF || item_id == 0xFFFFFFFF)
        return (uint32_t)-1;

    IdbEntry *entry = game_gloves_idb->FindEntryByID(item_id);
    if (entry)
        return entry->model;

    return (uint32_t)-1;
}

uint32_t Cac2X2m::BootsItem(uint32_t item_id)
{
    if (item_id == 0xFFFF || item_id == 0xFFFFFFFF)
        return (uint32_t)-1;

    IdbEntry *entry = game_shoes_idb->FindEntryByID(item_id);
    if (entry)
        return entry->model;

    return (uint32_t)-1;
}

uint32_t Cac2X2m::AccesoryItem(uint32_t item_id)
{
    if (item_id == 0xFFFF || item_id == 0xFFFFFFFF)
        return (uint32_t)-1;

    IdbEntry *entry = game_accesory_idb->FindEntryByID(item_id);
    if (entry)
        return entry->model;

    return (uint32_t)-1;
}

bool Cac2X2m::SetCms(X2mFile *x2m, const std::string &cms_entry, std::unordered_set<std::string> &files, std::string &base_dir, BcsFile &bcs, EskFile &esk, EanFile &cam_ean)
{
    CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByName(cms_entry));
    if (!cms)
        return false;

    CmsEntryXV2 new_cms = *cms;
    new_cms.name = x2m->GetEntryName();

    const std::string char_dir = std::string("data/chara/") + std::string(cms_entry) + "/";
    const std::string character = char_dir + cms->character;

    base_dir = Utils::NormalizePath(character);
    base_dir = base_dir.substr(0, base_dir.rfind('/') + 1);

    const std::string bcs_path = character + ".bcs";

    if (!xv2fs->LoadFile(&bcs, bcs_path))
    {
        DPRINTF("%s: Failed to load bcs file \"%s\" from game.\n", FUNCNAME, bcs_path.c_str());
        return false;
    }

    const std::string esk_path = character + "_000.esk";

    if (!xv2fs->LoadFile(&esk, esk_path))
    {
        DPRINTF("%s: Failed to load esk file \"%s\" from game.\n", FUNCNAME, esk_path.c_str());
        return false;
    }

    new_cms.character = new_cms.name;

    if (cms->ean.length() > 0)
    {
        add_file(char_dir + cms->ean + ".ean", files, false);
        new_cms.ean = new_cms.name;
    }

    if (cms->fce_ean.length() > 0)
    {
        add_file(char_dir + cms->fce_ean + ".fce.ean", files, false);
        convert_str_ref(new_cms.fce_ean, cms->fce_ean, new_cms.name);
    }

    if (cms->fce.length() > 0)
    {
        add_file(char_dir + cms->fce + ".ean", files, false);
        convert_str_ref(new_cms.fce, cms->fce, new_cms.name);
    }

    if (cms->cam_ean.length() > 0)
    {
        const std::string cam_ean_path = character + ".cam.ean";

        if (!xv2fs->LoadFile(&cam_ean, cam_ean_path))
        {
            DPRINTF("%s: Failed to load .cam.ean file \"%s\" from game.\n", FUNCNAME, cam_ean_path.c_str());
            return false;
        }

        new_cms.cam_ean = new_cms.name;
    }

    if (cms->bac.length() > 0)
    {
        add_file(char_dir + cms->bac + "_PLAYER.bac", files, false);
        new_cms.bac = new_cms.name;
    }

    if (cms->bcm.length() > 0)
    {
        add_file(char_dir + cms->bcm + "_PLAYER.bcm", files, false);
        new_cms.bcm = new_cms.name;
    }

    if (cms->bdm.length() > 0)
    {
        add_file(char_dir + cms->bdm + "_PLAYER.bdm", files, false);
        new_cms.bdm = new_cms.name;
    }

    CmsEntryXV2 &x2m_cms = x2m->GetCmsEntry();
    x2m_cms = new_cms;

    return true;
}

bool Cac2X2m::SetCms(X2mFile *x2m, uint32_t cms_entry, std::unordered_set<std::string> &files, std::string &base_dir, BcsFile &bcs, EskFile &esk, EanFile &cam_ean)
{
    CmsEntry *cms = game_cms->FindEntryByID(cms_entry);
    if (!cms)
        return false;

    return SetCms(x2m, cms->name, files, base_dir, bcs, esk, cam_ean);
}

static uint8_t *GetCustomSkillGuid(uint16_t id, uint8_t *out, X2mSkillType type, CusSkill **pskill)
{
    if (id == 0xFFFF)
        return nullptr;

    CusSkill *skill = game_cus->FindSkillAnyByID(id);
    if (!skill)
        return nullptr;

    if (pskill)
        *pskill = skill;

    CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(skill->id2 / 10));
    if (!cms || cms->unk_10 != X2M_DUMMY_ID)
        return nullptr;

    std::string ini_path = Utils::MakePathString(X2mFile::GetSkillDirectory(*skill, type), X2M_SKILL_INI);
    IniFile ini;
    std::string guid_str;

    if (!xv2fs->LoadFile(&ini, ini_path))
        return nullptr;

    if (ini.GetStringValue("General", "GUID", guid_str))
    {
        if (Utils::String2GUID(out, guid_str))
            return out;
    }

    return nullptr;
}

void Cac2X2m::ResolveCustomSkills(X2mFile *x2m, CusSkillSet &set)
{
    for (int i = 0; i < 9; i++)
    {
        X2mSkillType type;
        uint8_t guid[16];
        CusSkill *skill;

        if (i < 4)
            type = X2mSkillType::SUPER;
        else if (i < 6)
            type = X2mSkillType::ULTIMATE;
        else if (i == 6)
            type = X2mSkillType::EVASIVE;
        else if (i == 7)
            type = X2mSkillType::BLAST;
        else
            type = X2mSkillType::AWAKEN;

        if (GetCustomSkillGuid(set.char_skills[i], guid, type, &skill))
        {
            X2mDepends *dep = x2m->FindCharaSkillDepends(guid);
            if (dep)
            {
                set.char_skills[i] = (uint16_t)dep->id;
            }
            else
            {
                std::string name = skill->name;

                if (type == X2mSkillType::SUPER)
                {
                    Xenoverse2::GetSuperSkillName(skill->id2, name);
                }
                else if (type == X2mSkillType::ULTIMATE)
                {
                    Xenoverse2::GetUltimateSkillName(skill->id2, name);
                }
                else if (type == X2mSkillType::EVASIVE)
                {
                    Xenoverse2::GetEvasiveSkillName(skill->id2, name);
                }
                else if (type == X2mSkillType::BLAST)
                {
                    name = "Blast: " + skill->name;
                }
                else if (type == X2mSkillType::AWAKEN)
                {
                    Xenoverse2::GetAwakenSkillName(skill->id2, name);
                }

                dep = x2m->AddCharaSkillDepends(guid, name);
                if (dep)
                {
                    set.char_skills[i] = (uint16_t)dep->id;
                }
            }
        }
    }
}

bool Cac2X2m::SetCso(uint32_t race, CsoEntry &new_cso, std::unordered_set<std::string> &files, uint32_t voice)
{
    std::vector<CsoEntry *> cso_entries;
    CmsEntry *cms = game_cms->FindEntryByID(race);
    if (!cms)
        return false;

    bool female = (cms->name == "HUF" || cms->name == "SYF" || cms->name == "MAF");

    if (game_cso->FindEntriesByCharID(race, cso_entries) == 0)
    {
        DPRINTF("%s: Failed to get original cso entry of race.\n", FUNCNAME);
        return false;
    }

    new_cso = *(cso_entries[0]);

    new_cso.skills.clear();
    new_cso.skills.push_back((female) ? 'F' : 'M');
    new_cso.skills += Utils::ToStringAndPad((int)voice, 2);

    new_cso.vox = "CAR_BTL_" + new_cso.skills + "_VOX";

    if (new_cso.amk.length() > 0)
    {
        add_file("data/chara/" + new_cso.amk + ".amk", files, false);
        new_cso.amk.clear();
    }
    else
    {
        add_file("data/chara/" + cms->name + "/" + cms->name + ".amk", files, false);
    }

    return true;
}

bool Cac2X2m::SetCso(const std::string &race, CsoEntry &new_cso, std::unordered_set<std::string> &files, uint32_t voice)
{
    CmsEntry *cms = game_cms->FindEntryByName(race);
    if (!cms)
        return false;

    return SetCso(cms->id, new_cso, files, voice);
}

bool Cac2X2m::SetPsc(uint32_t race, PscSpecEntry &new_psc, uint32_t body_shape, uint16_t hea, uint16_t ki, uint16_t stm, uint16_t atk, uint16_t str, uint16_t bla, uint32_t talisman)
{
    PscSpecEntry *orig_psc = game_psc->FindSpec(race, body_shape, 0);
    if (!orig_psc)
    {
        DPRINTF("%s: Cannot find psc entry for race 0x%x and body shape %d.\n", FUNCNAME, race, body_shape);
        return false;
    }

    new_psc = *orig_psc; // Copy

    new_psc.health += game_psa->GetHea(hea);   
    new_psc.ki += game_psa->GetKi(ki);
    new_psc.stamina += game_psa->GetSta(stm);
    new_psc.basic_attack += game_psa->GetAtk(atk);
    new_psc.strike_attack += game_psa->GetStr(str);
    new_psc.ki_blast_super += game_psa->GetBla(bla);

    if (talisman == 0xFFFF)
        new_psc.talisman = 0xFFFFFFFF;
    else
        new_psc.talisman = talisman;

    return true;
}

bool Cac2X2m::SetPsc(const std::string &race, PscSpecEntry &new_psc, uint32_t body_shape, uint16_t hea, uint16_t ki, uint16_t stm, uint16_t atk, uint16_t str, uint16_t bla, uint32_t talisman)
{
    CmsEntry *cms = game_cms->FindEntryByName(race);
    if (!cms)
        return false;

    return SetPsc(cms->id, new_psc, body_shape, hea, ki, stm, atk, str, bla, talisman);
}

bool Cac2X2m::SetAura(uint32_t race, AurCharaLink &new_aur)
{
    std::vector<AurCharaLink *> links;

    if (game_aur->FindCharaLinks(race, links) == 0)
    {
        DPRINTF("%s: Cannot find aur entry for race 0x%x\n", race);
        return false;
    }

    new_aur = *(links[0]); // copy
    return true;
}

bool Cac2X2m::SetAura(const std::string &race, AurCharaLink &new_aur)
{
    CmsEntry *cms = game_cms->FindEntryByName(race);
    if (!cms)
        return false;

    return SetAura(cms->id, new_aur);
}

bool Cac2X2m::SetCml(uint32_t race, CmlEntry &new_cml, uint32_t body_shape)
{
    CmlEntry *orig_cml = game_cml->FindEntry(race, body_shape / 3);
    if (!orig_cml)
    {
        DPRINTF("%s: Cannot find cml entry for race 0x%x body_shape %d.\n", FUNCNAME, race, body_shape);
        return false;
    }

    new_cml = *orig_cml; // Copy
    return true;
}

bool Cac2X2m::SetCml(const std::string &race, CmlEntry &new_cml, uint32_t body_shape)
{
    CmsEntry *cms = game_cms->FindEntryByName(race);
    if (!cms)
        return false;

    return SetCml(cms->id, new_cml, body_shape);
}

bool Cac2X2m::WriteFiles(X2mFile *x2m, const std::unordered_set<std::string> &files)
{
    const std::string &entry_name = x2m->GetEntryName();

    for (const std::string &file : files)
    {
        size_t size;
        uint8_t *buf = xv2fs->ReadFile(file, &size);
        if (!buf)
        {
            DPRINTF("%s: Failed to read game file \"%s\"\n", FUNCNAME, file.c_str());
            return false;
        }

        std::string output;
        convert_str_ref(output, file, entry_name);
        output = entry_name + "/" + output;

        bool ret = x2m->WriteFile(output, buf, size);
        delete[] buf;

        if (!ret)
        {
            DPRINTF("%s: Failed to write file \"%s\" to the x2m.\n", FUNCNAME, output.c_str());
            return false;
        }
    }

    return true;
}

bool Cac2X2m::WriteCustomFiles(X2mFile *x2m, BcsFile &bcs, EskFile &esk, EanFile &cam_ean)
{
    const std::string bcs_path = x2m->GetEntryName() + "/" + x2m->GetEntryName() + ".bcs";
    size_t size;
    uint8_t *buf = bcs.Save(&size);

    if (!buf)
    {
        DPRINTF("%s: Internal error, bcs save failed.\n", FUNCNAME);
        return false;
    }

    bool ret = x2m->WriteFile(bcs_path, buf, size);
    delete[] buf;

    if (!ret)
    {
        DPRINTF("%s: Failed to write bcs file to x2m (\"%s\")\n", FUNCNAME, bcs_path.c_str());
        return false;
    }

    const std::string esk_path = x2m->GetEntryName() + "/" + x2m->GetEntryName() + "_000.esk";
    buf = esk.Save(&size);

    if (!buf)
    {
        DPRINTF("%s: Internal error, esk save failed.\n", FUNCNAME);
        return false;
    }

    ret = x2m->WriteFile(esk_path, buf, size);
    delete[] buf;

    if (!ret)
    {
        DPRINTF("%s: Failed to write esk file to x2m (\"%s\")\n", FUNCNAME, esk_path.c_str());
        return false;
    }

    const std::string cam_ean_path = x2m->GetEntryName() + "/" + x2m->GetEntryName() + ".cam.ean";
    buf = cam_ean.Save(&size);

    if (!buf)
    {
        DPRINTF("%s: Internal error, cam.ean save failed.\n", FUNCNAME);
        return false;
    }

    ret = x2m->WriteFile(cam_ean_path, buf, size);
    delete[] buf;

    if (!ret)
    {
        DPRINTF("%s: Failed to write cam.ean file to x2m (\"%s\")\n", FUNCNAME, cam_ean_path.c_str());
        return false;
    }

    return ret;
}

bool Cac2X2m::WriteSelPort(X2mFile *x2m, const std::string &cms_entry)
{
    uint32_t emb_idx = game_sel_port->FindIndexByName(cms_entry + ".dds");
    if (emb_idx == 0xFFFF)
    {
        DPRINTF("%s: Cannot find race sel dds (race=%s).\n", FUNCNAME, cms_entry.c_str());
        return false;
    }

    const EmbContainedFile &file = (*game_sel_port)[emb_idx];
    if (!x2m->WriteFile(X2M_SEL_PORTRAIT, file.GetData(), file.GetSize()))
    {
        DPRINTF("%s: Failed to write sel portrait to x2m.\n", FUNCNAME);
        return false;
    }

    return true;
}

bool Cac2X2m::WriteSelPort(X2mFile *x2m, uint32_t cms_entry)
{
    CmsEntry *cms = game_cms->FindEntryByID(cms_entry);
    if (!cms)
        return false;

    return WriteSelPort(x2m, cms->name);
}

bool Cac2X2m::WriteBtlPort(X2mFile *x2m, const std::string &cms_entry)
{
    EmbFile emb;
    EmbContainedFile file;

    uint32_t emb_idx = game_sel_port->FindIndexByName(cms_entry + ".dds");
    if (emb_idx == 0xFFFF)
    {
        DPRINTF("%s: Cannot find race sel dds (race=%s).\n", FUNCNAME, cms_entry.c_str());
        return false;
    }

    file = (*game_sel_port)[emb_idx]; // Copy
    file.SetName(x2m->GetEntryName() + "_000.dds");

    emb.AppendFile(file);

    size_t size;
    uint8_t *buf = emb.Save(&size);
    if (!buf)
    {
        DPRINTF("%s: Failed to create btl portrait.\n", FUNCNAME);
        return false;
    }

    bool ret = x2m->WriteFile(X2M_BTL_PORTRAIT, buf, size);
    if (!ret)
    {
        DPRINTF("%s: Failed to write btl portait to x2m.\n", FUNCNAME);
        return false;
    }

    return true;
}

bool Cac2X2m::WriteBtlPort(X2mFile *x2m, uint32_t cms_entry)
{
    CmsEntry *cms = game_cms->FindEntryByID(cms_entry);
    if (!cms)
        return false;

    return WriteBtlPort(x2m, cms->name);
}

uint16_t Cac2X2m::SuperSkillFromId2(uint16_t id2)
{
    if (id2 == 0xFFFF)
        return 0xFFFF;

    const std::vector<CusSkill> &skills = game_cus->GetSuperSkills();
    for (const CusSkill &skill : skills)
    {
        if (skill.id2 == id2)
            return skill.id;
    }

    return 0xFFFF;
}

uint16_t Cac2X2m::UltSkillFromId2(uint16_t id2)
{
    if (id2 == 0xFFFF)
        return 0xFFFF;

    const std::vector<CusSkill> &skills = game_cus->GetUltimateSkills();
    for (const CusSkill &skill : skills)
    {
        if (skill.id2 == id2)
            return skill.id;
    }

    return 0xFFFF;
}

uint16_t Cac2X2m::EvaSkillFromId2(uint16_t id2)
{
    if (id2 == 0xFFFF)
        return 0xFFFF;

    const std::vector<CusSkill> &skills = game_cus->GetEvasiveSkills();
    for (const CusSkill &skill : skills)
    {
        if (skill.id2 == id2)
            return skill.id;
    }

    return 0xFFFF;
}

uint16_t Cac2X2m::BlastSkillFromId2(uint16_t id2)
{
    if (id2 == 0xFFFF)
        return 0xFFFF;

    const std::vector<CusSkill> &skills = game_cus->GetBlastSkills();
    for (const CusSkill &skill : skills)
    {
        if (skill.id2 == id2)
            return skill.id;
    }

    return 0xFFFF;
}

uint16_t Cac2X2m::AwakenSkillFromId2(uint16_t id2)
{
    if (id2 == 0xFFFF)
        return 0xFFFF;

    const std::vector<CusSkill> &skills = game_cus->GetAwakenSkills();
    for (const CusSkill &skill : skills)
    {
        if (skill.id2 == id2)
            return skill.id;
    }

    return 0xFFFF;
}

static bool WriteVoice(X2mFile *x2m, const std::string &voice_name, const AcbFile &acb, const AwbFile &awb, const std::string &out_path)
{
    uint32_t cue_id = acb.FindCueId(voice_name);
    if (cue_id == (uint32_t)-1)
    {
        DPRINTF("%s: Failed to find voice \"%s\"\n", FUNCNAME, voice_name.c_str());
        return false;
    }

    uint32_t cue_idx = acb.CueIdToCueIndex(cue_id);
    if (cue_idx == (uint32_t)-1)
        return false;

    bool external;
    uint32_t awb_index = acb.CueIndexToAwbIndex(cue_idx, &external);
    if (awb_index == (uint32_t)-1 || !external)
    {
        DPRINTF("%s: Failed to map voice \"%s\" to awb index or track is internal.\n", FUNCNAME, voice_name.c_str());
        return false;
    }

    uint64_t size;
    uint8_t *buf = awb.ExtractFile(awb_index, &size);
    if (!buf)
    {
        DPRINTF("%s: Failed to get file with index %d from awb.\n", FUNCNAME, awb_index);
        return false;
    }

    bool ret = x2m->WriteFile(out_path, buf, size);
    delete[] buf;

    if (!ret)
    {
        DPRINTF("%s: Failed to write data to x2m.\n", FUNCNAME);
    }

    return ret;
}

bool Cac2X2m::WriteCssVoice(X2mFile *x2m, uint32_t race, uint32_t voice)
{
    static const std::vector<std::string> male_voices =
    {
        "BDQR_B01_000",
        "BDQR_B04_000",
        "BDQR_B05_000",
        "BDQR_M01_000",
        "BDQR_M02_000",
        "BDQR_M03_000",
        "BDQR_M05_000",
        "BDQR_M06_000",
        "BDQR_M07_000",
        "BDQR_M09_000",
        "CAQR_M10_000",
        "CAQR_M11_000",
        "CAQR_M12_000",
        "CAQR_M13_000",
        "CAQR_M14_000"
    };

    static const std::vector<std::string> female_voices =
    {
        "BDQR_G01_000",
        "BDQR_G02_000",
        "BDQR_G03_000",
        "BDQR_G04_000",
        "BDQR_G05_000",
        "BDQR_G06_000",
        "BDQR_W01_000",
        "BDQR_W06_000",
        "BDQR_W07_000",
        "BDQR_W08_000",
        "CAQR_F10_000",
        "CAQR_F11_000",
        "CAQR_F12_000",
        "CAQR_F13_000",
        "CAQR_F14_000"
    };

    CmsEntry *cms = game_cms->FindEntryByID(race);
    if (!cms)
        return false;

    bool female = (cms->name == "HUF" || cms->name == "SYF" || cms->name == "MAF");

    if (male_voices.size() != 15 || female_voices.size() != 15)
    {
        DPRINTF("Buuuu\n");
        exit(-1);
    }

    if (voice > male_voices.size())
    {
        DPRINTF("%s: Voice %d is out of bounds.\n", voice);
        return false;
    }

    const std::string &voice_name = (female) ? female_voices[voice] : male_voices[voice];

    AcbFile caqr_acb, caqr_acb_en;
    Afs2File caqr_awb, caqr_awb_en;

    if (!xv2fs->LoadFile(dynamic_cast<CriwareAudioContainer *>(&caqr_acb), std::string(CAQR_PATH) + ".acb"))
    {
        DPRINTF("%s failed to load japanese caqr acb.\n", FUNCNAME);
        return false;
    }

    if (!xv2fs->LoadFile(&caqr_awb, std::string(CAQR_PATH) + ".awb"))
    {
        DPRINTF("%s failed to load japanese caqr awb.\n", FUNCNAME);
        return false;
    }

    if (!xv2fs->LoadFile(dynamic_cast<CriwareAudioContainer *>(&caqr_acb_en), std::string(CAQR_EN_PATH) + ".acb"))
    {
        DPRINTF("%s failed to load english caqr acb.\n", FUNCNAME);
        return false;
    }

    if (!xv2fs->LoadFile(&caqr_awb_en, std::string(CAQR_EN_PATH) + ".awb"))
    {
        DPRINTF("%s failed to load english caqr awb.\n", FUNCNAME);
        return false;
    }

    if (!WriteVoice(x2m, voice_name, caqr_acb, caqr_awb, std::string(X2M_AUDIO) + "VOICE_jp.hca"))
    {
        DPRINTF("%s: WriteVoice failed in japanese track.\n", FUNCNAME);
        return false;
    }

    if (!WriteVoice(x2m, voice_name, caqr_acb_en, caqr_awb_en, std::string(X2M_AUDIO) + "VOICE_en.hca"))
    {
        DPRINTF("%s: WriteVoice failed in english track.\n", FUNCNAME);
        return false;
    }

    return true;
}

bool Cac2X2m::WriteCssVoice(X2mFile *x2m, const std::string &race, uint32_t voice)
{
    CmsEntry *cms = game_cms->FindEntryByName(race);
    if (!cms)
        return false;

    return WriteCssVoice(x2m, cms->id, voice);
}

struct Md5Pair
{
    uint8_t md5[16];
    uint32_t model;
};

const std::vector<Md5Pair> md5_pants =
{
    { { 0xCD, 0xC8, 0xF7, 0xBE, 0x71, 0x21, 0x77, 0x17, 0x44, 0x95, 0x93, 0xD1, 0x0F, 0x15, 0xA5, 0x6A }, 300 },
    { { 0xB7, 0xD2, 0x49, 0x4E, 0x1F, 0xBA, 0x9E, 0xA3, 0x6A, 0x6D, 0xEF, 0x55, 0x4A, 0xCD, 0x53, 0xC8 }, 523 }
};

const std::vector<Md5Pair> md5_boots =
{
    { { 0x06, 0x4A, 0xD2, 0x12, 0x79, 0x72, 0xA8, 0x01, 0xC8, 0x97, 0x57, 0xF1, 0xF7, 0xB8, 0xC3, 0x14 }, 518 }
};

bool X2m2Cac::GetRace(X2mFile *x2m, std::string &race)
{
    if (x2m->GetType() != X2mType::NEW_CHARACTER)
        return false;

    if (!x2m->HasCso() || !x2m->UsesBodyShape())
        return false;

    if (!x2m->FileExists(Utils::MakePathString(x2m->GetEntryName(), x2m->GetEntryName() + ".bcs")))
        return false;

    const CsoEntry &cso = x2m->GetCsoEntry(0);

    if (cso.se.length() == 0)
    {
        // Case of sym

        if (cso.skills.length() != 3)
            return false;

        if (cso.skills[0] != 'M')
            return false;

        if (cso.skills[1] < '0' || cso.skills[1] > '9')
            return false;

        if (cso.skills[2] < '0' || cso.skills[2] > '9')
            return false;

        race = "SYM";
        return true;
    }

    if (cso.se.length() != 14)
        return false;

    if (cso.se.substr(0, 8) != "CAR_BTL_" || cso.se.substr(12, 2) != "SE")
        return false;

    race = cso.se.substr(8, 3);

    if (race != "HUM" && race != "HUF" && race != "SYM" && race != "SYF" && race != "NMC" && race != "FRI" && race != "MAM" && race != "MAF")
        return false;

    return true;
}

int X2m2Cac::GetVoice(X2mFile *x2m)
{
    if (x2m->GetType() != X2mType::NEW_CHARACTER)
        return -1;

    if (!x2m->HasCso())
        return -1;

    const CsoEntry &cso = x2m->GetCsoEntry(0);
    if (cso.skills.length() != 3)
        return -1;

    if (cso.skills[0] != 'M' && cso.skills[0] != 'F')
        return -1;

    if (cso.skills[1] < '0' || cso.skills[1] > '9')
        return -1;

    if (cso.skills[2] < '0' || cso.skills[2] > '9')
        return -1;

    size_t offset = (cso.skills[1] == '0') ? 2 : 1;
    int voice = Utils::GetSigned(cso.skills.substr(offset));

    if (voice > 14)
        return -1;

    return voice;
}

bool X2m2Cac::GetName(X2mFile *x2m, std::string &name, int lang)
{
    if (x2m->GetType() != X2mType::NEW_CHARACTER)
        return false;

    if (lang < 0 || lang >= XV2_LANG_NUM)
        return false;

    name = x2m->GetCharaName(lang);
    if (lang != XV2_LANG_ENGLISH && name.length() == 0)
        name = x2m->GetCharaName(XV2_LANG_ENGLISH);

    return true;
}

uint32_t X2m2Cac::BustItem(uint32_t model, const std::string &race)
{
    if (model == 0xFFFF || model == 0xFFFFFFFF)
        return (uint32_t)-1;

    uint32_t race_idb;

    if (race == "HUM")
    {
        race_idb = IDB_RACE_HUM;
    }
    else if (race == "HUF")
    {
        race_idb = IDB_RACE_HUF;
    }
    else if (race == "SYM")
    {
        race_idb = IDB_RACE_SYM;
    }
    else if (race == "SYF")
    {
        race_idb = IDB_RACE_SYF;
    }
    else if (race == "NMC")
    {
        race_idb = IDB_RACE_NMC;
    }
    else if (race == "FRI")
    {
        race_idb = IDB_RACE_FRI;
    }
    else if (race == "MAM")
    {
        race_idb = IDB_RACE_MAM;
    }
    else
    {
        race_idb = IDB_RACE_MAF;
    }

    const std::vector<IdbEntry> &entries = game_top_idb->GetEntries();

    for (const IdbEntry &entry : entries)
    {
        bool bypass = false;

        if (!(entry.racelock & race_idb))
        {
            if (race == "HUM")
            {
                if (entry.racelock & IDB_RACE_SYM)
                    bypass = true;
            }
            else if (race == "HUF")
            {
                if (entry.racelock & IDB_RACE_SYF)
                    bypass = true;
            }
        }

        if (bypass || (entry.racelock & race_idb))
        {
            if (entry.model == model)
                return entry.id;
        }
    }

    return (uint32_t)-1;
}

uint32_t X2m2Cac::PantsItem(uint32_t model, const std::string &race)
{
    if (model == 0xFFFF || model == 0xFFFFFFFF)
        return (uint32_t)-1;

    uint32_t race_idb;

    if (race == "HUM")
    {
        race_idb = IDB_RACE_HUM;
    }
    else if (race == "HUF")
    {
        race_idb = IDB_RACE_HUF;
    }
    else if (race == "SYM")
    {
        race_idb = IDB_RACE_SYM;
    }
    else if (race == "SYF")
    {
        race_idb = IDB_RACE_SYF;
    }
    else if (race == "NMC")
    {
        race_idb = IDB_RACE_NMC;
    }
    else if (race == "FRI")
    {
        race_idb = IDB_RACE_FRI;
    }
    else if (race == "MAM")
    {
        race_idb = IDB_RACE_MAM;
    }
    else
    {
        race_idb = IDB_RACE_MAF;
    }

    const std::vector<IdbEntry> &entries = game_bottom_idb->GetEntries();

    for (const IdbEntry &entry : entries)
    {
        bool bypass = false;

        if (!(entry.racelock & race_idb))
        {
            if (race == "HUM")
            {
                if (entry.racelock & IDB_RACE_SYM)
                    bypass = true;
            }
            else if (race == "HUF")
            {
                if (entry.racelock & IDB_RACE_SYF)
                    bypass = true;
            }
        }

        if (bypass || (entry.racelock & race_idb))
        {
            if (entry.model == model)
                return entry.id;
        }
    }

    return (uint32_t)-1;
}

uint32_t X2m2Cac::RistItem(uint32_t model, const std::string &race)
{
    if (model == 0xFFFF || model == 0xFFFFFFFF)
        return (uint32_t)-1;

    uint32_t race_idb;

    if (race == "HUM")
    {
        race_idb = IDB_RACE_HUM;
    }
    else if (race == "HUF")
    {
        race_idb = IDB_RACE_HUF;
    }
    else if (race == "SYM")
    {
        race_idb = IDB_RACE_SYM;
    }
    else if (race == "SYF")
    {
        race_idb = IDB_RACE_SYF;
    }
    else if (race == "NMC")
    {
        race_idb = IDB_RACE_NMC;
    }
    else if (race == "FRI")
    {
        race_idb = IDB_RACE_FRI;
    }
    else if (race == "MAM")
    {
        race_idb = IDB_RACE_MAM;
    }
    else
    {
        race_idb = IDB_RACE_MAF;
    }

    const std::vector<IdbEntry> &entries = game_gloves_idb->GetEntries();

    for (const IdbEntry &entry : entries)
    {
        bool bypass = false;

        if (!(entry.racelock & race_idb))
        {
            if (race == "HUM")
            {
                if (entry.racelock & IDB_RACE_SYM)
                    bypass = true;
            }
            else if (race == "HUF")
            {
                if (entry.racelock & IDB_RACE_SYF)
                    bypass = true;
            }
        }

        if (bypass || (entry.racelock & race_idb))
        {
            if (entry.model == model)
                return entry.id;
        }
    }

    return (uint32_t)-1;
}

uint32_t X2m2Cac::BootsItem(uint32_t model, const std::string &race)
{
    if (model == 0xFFFF || model == 0xFFFFFFFF)
        return (uint32_t)-1;

    uint32_t race_idb;

    if (race == "HUM")
    {
        race_idb = IDB_RACE_HUM;
    }
    else if (race == "HUF")
    {
        race_idb = IDB_RACE_HUF;
    }
    else if (race == "SYM")
    {
        race_idb = IDB_RACE_SYM;
    }
    else if (race == "SYF")
    {
        race_idb = IDB_RACE_SYF;
    }
    else if (race == "NMC")
    {
        race_idb = IDB_RACE_NMC;
    }
    else if (race == "FRI")
    {
        race_idb = IDB_RACE_FRI;
    }
    else if (race == "MAM")
    {
        race_idb = IDB_RACE_MAM;
    }
    else
    {
        race_idb = IDB_RACE_MAF;
    }

    const std::vector<IdbEntry> &entries = game_shoes_idb->GetEntries();

    for (const IdbEntry &entry : entries)
    {
        bool bypass = false;

        if (!(entry.racelock & race_idb))
        {
            if (race == "HUM")
            {
                if (entry.racelock & IDB_RACE_SYM)
                    bypass = true;
            }
            else if (race == "HUF")
            {
                if (entry.racelock & IDB_RACE_SYF)
                    bypass = true;
            }
        }

        if (bypass || (entry.racelock & race_idb))
        {
            if (entry.model == model)
                return entry.id;
        }
    }

    return (uint32_t)-1;
}

uint32_t X2m2Cac::AccesoryItem(uint32_t model, const std::string &race)
{
    if (model == 0xFFFF || model == 0xFFFFFFFF)
        return (uint32_t)-1;

    uint32_t race_idb;

    if (race == "HUM")
    {
        race_idb = IDB_RACE_HUM;
    }
    else if (race == "HUF")
    {
        race_idb = IDB_RACE_HUF;
    }
    else if (race == "SYM")
    {
        race_idb = IDB_RACE_SYM;
    }
    else if (race == "SYF")
    {
        race_idb = IDB_RACE_SYF;
    }
    else if (race == "NMC")
    {
        race_idb = IDB_RACE_NMC;
    }
    else if (race == "FRI")
    {
        race_idb = IDB_RACE_FRI;
    }
    else if (race == "MAM")
    {
        race_idb = IDB_RACE_MAM;
    }
    else
    {
        race_idb = IDB_RACE_MAF;
    }

    const std::vector<IdbEntry> &entries = game_accesory_idb->GetEntries();

    for (const IdbEntry &entry : entries)
    {
        bool bypass = false;

        if (!(entry.racelock & race_idb))
        {
            if (race == "HUM")
            {
                if (entry.racelock & IDB_RACE_SYM)
                    bypass = true;
            }
            else if (race == "HUF")
            {
                if (entry.racelock & IDB_RACE_SYF)
                    bypass = true;
            }
        }

        if (bypass || (entry.racelock & race_idb))
        {
            if (entry.model == model)
                return entry.id;
        }
    }

    return (uint32_t)-1;
}

bool X2m2Cac::GetColor(const BcsFile &bcs, const BcsPart &part, const std::string &part_name, uint32_t *color)
{
    uint32_t idx = bcs.FindPartColorsByName(part_name);
    bool found = false;

    if (idx != (uint32_t)-1)
    {
        for (const BcsColorSelector &sel : part.color_selectors)
        {
            if (sel.part_colors == idx)
            {
                *color = sel.color;
                found = true; // Don't return yet, we want to return last item that matches
            }
        }
    }

    return found;
}

bool X2m2Cac::GetPart(X2mFile *x2m, const BcsFile &race_bcs, uint32_t slot_idx, uint32_t part_idx, uint32_t *part_out, uint32_t *colors_out, bool *is_accessory_out)
{
    std::string race;

    if (!GetRace(x2m, race))
    {
        DPRINTF("%s: Failed to get race.\n", FUNCNAME);
        return false;
    }

    if (race == "SYM")
        race = "HUM";
    else if (race == "SYF")
        race = "HUF";

    size_t bcs_size;
    uint8_t *bcs_buf = x2m->ReadFile(Utils::MakePathString(x2m->GetEntryName(), x2m->GetEntryName() + ".bcs"), &bcs_size);

    if (!bcs_buf)
    {
        DPRINTF("%s: Cannot read bcs file.\n", FUNCNAME);
        return false;
    }

    BcsFile x2m_bcs;
    bool ret = x2m_bcs.Load(bcs_buf, bcs_size);
    delete[] bcs_buf;

    if (!ret)
    {
        DPRINTF("%s: Cannot load bcs file.\n", FUNCNAME);
        return false;
    }

    const X2mSlotEntry &slot_entry = x2m->GetSlotEntry(slot_idx);
    const std::vector<BcsPartSet> &x2m_partsets = x2m_bcs.GetPartSets();

    if (slot_entry.costume_index >= x2m_partsets.size())
    {
        DPRINTF("%s: costume_index %d doesn't exist in bcs.\n", FUNCNAME, slot_entry.costume_index);
        return false;
    }

    const BcsPartSet &x2m_partset = x2m_partsets[slot_entry.costume_index];

    if (!x2m_partset.valid)
    {
        DPRINTF("%s: Partset %d is empty.\n", FUNCNAME, slot_entry.costume_index);
        return false;
    }

    if (part_idx >= x2m_partset.parts.size() || part_idx >= part_names.size())
    {
        DPRINTF("%s: Part %d is out of bounds.\n", FUNCNAME, part_idx);
        return false;
    }

    const BcsPart &x2m_part = x2m_partset.parts[part_idx];
    if (!x2m_part.valid)
    {
        DPRINTF("%s: Part %d in partset %d is empty.\n", FUNCNAME, part_idx, slot_entry.costume_index);
        return false;
    }

    bool null_model = false;

    if (x2m_part.model == 0xFFFF)
    {
        if (part_idx == PART_FACE_EYE || part_idx == PART_FACE_NOSE)
        {
            null_model = true;
        }
        else
        {
            DPRINTF("%s: model is 0xFFFF, cannot proceed.\n", FUNCNAME);
            return false;
        }
    }

    std::string file_path_x2m = x2m->GetEntryName() + "_" + Utils::ToStringAndPad(x2m_part.model, 3) + "_" + part_names[part_idx] + ".emd";
    file_path_x2m = Utils::MakePathString(x2m->GetEntryName(), file_path_x2m);

    //DPRINTF("%s\n", file_path_x2m.c_str());

    size_t emd_size_x2m;
    uint8_t *emd_buf_x2m;

    size_t emd_size_x2m_ph = 0;
    uint8_t *emd_buf_x2m_ph = nullptr;

    EmbFile *dyt_emb_x2m = nullptr;

    if (null_model)
    {
        emd_size_x2m = 0;
        emd_buf_x2m = nullptr;

        emd_size_x2m_ph = 0;
        emd_buf_x2m_ph = nullptr;
    }
    else
    {
        emd_buf_x2m = x2m->ReadFile(file_path_x2m, &emd_size_x2m);
        if (!emd_buf_x2m)
        {
            DPRINTF("%s: Failed to find file \"%s\" in x2m.\n", FUNCNAME, file_path_x2m.c_str());
            return false;
        }

        if (part_idx == PART_HAIR && x2m_part.model == 0 && x2m_part.physics.size() > 0)
        {
            const BcsPhysics &ph = x2m_part.physics[0];

            file_path_x2m = ph.unk_28[0] + ".emd";
            file_path_x2m = Utils::MakePathString(x2m->GetEntryName(), file_path_x2m);

            emd_buf_x2m_ph = x2m->ReadFile(file_path_x2m, &emd_size_x2m_ph);
        }

        if (part_idx >= PART_BUST)
        {
            file_path_x2m = x2m->GetEntryName() + "_" + Utils::ToStringAndPad(x2m_part.model, 3) + "_" + part_names[part_idx] + ".dyt.emb";
            file_path_x2m = Utils::MakePathString(x2m->GetEntryName(), file_path_x2m);

            //DPRINTF("%s\n", file_path_x2m.c_str());

            size_t dyt_size_x2m;
            uint8_t *dyt_buf_x2m = x2m->ReadFile(file_path_x2m, &dyt_size_x2m);

            if (dyt_buf_x2m)
            {
                dyt_emb_x2m = new EmbFile();
                bool ret = dyt_emb_x2m->Load(dyt_buf_x2m, dyt_size_x2m);
                delete[] dyt_buf_x2m;

                if (!ret)
                {
                    delete dyt_emb_x2m;
                    dyt_emb_x2m = nullptr;
                }
            }
        }
    }

    const std::vector<BcsPartSet> &race_partsets = race_bcs.GetPartSets();

    size_t start, limit;

    if (part_idx == PART_HAIR || part_idx >= PART_BUST)
    {
        limit = race_partsets.size();
    }
    else
    {
        limit = 300;

        if (limit > race_partsets.size())
            limit = race_partsets.size();
    }

    if (part_idx >= PART_BUST)
        start = 0;
    else
        start = 50;

    *is_accessory_out = false;

    size_t i;
    std::string file_path_race;
    int best_candidate = -1;

    for (i = start; i < limit; i++)
    {
        const BcsPartSet &race_partset = race_partsets[i];

        if (!race_partset.valid)
            continue;

        if (part_idx >= race_partset.parts.size())
            continue;

        const BcsPart &race_part = race_partset.parts[part_idx];
        if (!race_part.valid)
            continue;

        if (race_part.model == 0xFFFF)
        {
            if (null_model)
            {
                BcsPart test_part1 = x2m_part; // Copy
                BcsPart test_part2 = race_part; // Copy

                test_part1.color_selectors.clear();
                test_part2.color_selectors.clear();
                test_part1.unk3_data.clear();
                test_part2.unk3_data.clear();
                test_part1.num_unk3 = 0;
                test_part2.num_unk3 = 0;

                memcpy(test_part1.name, race.c_str(), 3);

                for (int f = 0; f < 4; f++)
                {
                    if (Utils::BeginsWith(test_part1.files[f], x2m->GetEntryName()))
                    {
                        test_part1.files[f] = race + test_part1.files[f].substr(3);
                    }
                }

                for (BcsPhysics &ph : test_part1.physics)
                {
                    memcpy(ph.name, race.c_str(), 3);

                    for (int f = 0; f < 6; f++)
                    {
                        if (Utils::BeginsWith(ph.unk_28[f], x2m->GetEntryName()))
                        {
                            ph.unk_28[f] = race + ph.unk_28[f].substr(3);
                        }
                    }
                }

                if (test_part1 == test_part2)
                {
                    goto check_label;
                }
            }

            continue;
        }

        if (null_model)
            continue;

        bool standard_files = true;

        if (i >= 300 && part_idx == PART_HAIR && emd_buf_x2m_ph && race_part.model == 0 && race_part.physics.size() > 0)
        {
            const BcsPhysics &ph = race_part.physics[0];

            file_path_race = ph.unk_28[0] + ".emd";
            file_path_race = Utils::MakePathString("/data/chara/" + race, file_path_race);

            if (xv2fs->FileExists(file_path_race))
            {
                standard_files = false;
            }
        }

        if (standard_files)
        {
            file_path_race = race + "_" + Utils::ToStringAndPad(race_part.model, 3) + "_" + part_names[part_idx] + ".emd";
            file_path_race = Utils::MakePathString("/data/chara/" + race, file_path_race);
        }

        size_t emd_size_race = xv2fs->GetFileSize(file_path_race);

        //DPRINTF("%s: %Id %Id\n", file_path_race.c_str(), emd_size_race, emd_size_x2m);
        if (standard_files)
        {
            if (emd_size_race != emd_size_x2m)
                continue;
        }
        else
        {
            if (emd_size_race != emd_size_x2m_ph)
                continue;
        }

        uint8_t *emd_buf_race = xv2fs->ReadFile(file_path_race, &emd_size_race);
        if (!emd_buf_race)
            continue;

        bool equal;

        if (standard_files)
            equal = (memcmp(emd_buf_x2m, emd_buf_race, emd_size_race) == 0);
        else
            equal = (memcmp(emd_buf_x2m_ph, emd_buf_race, emd_size_race) == 0);

        delete[] emd_buf_race;
        if (!equal)
            continue;

        if (part_idx >= PART_FACE_BASE && part_idx <= PART_HAIR)
        {
            BcsPart test_part = x2m_part; // copy

            bool continue_main_for = false;

            for (int f = 0; f < 4; f++)
            {
                if (test_part.files[f].length() > 0)
                {
                    if (race_part.files[f].length() == 0)
                    {
                        continue_main_for = true;
                        break;
                    }

                    if (Utils::BeginsWith(test_part.files[f], x2m->GetEntryName()))
                    {
                       test_part.files[f] = race + test_part.files[f].substr(3);
                    }
                }
                else
                {
                    if (race_part.files[f].length() != 0)
                    {
                        continue_main_for = true;
                        break;
                    }
                }

                if (test_part.files[f] != race_part.files[f])
                {
                    continue_main_for = true;
                    break;
                }
            }

            if (continue_main_for)
            {
                //DPRINTF("Dismissing %Id\n", i);
                continue;
            }
        }

        if (best_candidate < 0)
            best_candidate = (int)i;

        if (dyt_emb_x2m && part_idx >= PART_BUST)
        {
            file_path_race = race + "_" + Utils::ToStringAndPad(race_part.model, 3) + "_" + part_names[part_idx] + ".dyt.emb";
            file_path_race = Utils::MakePathString("/data/chara/" + race, file_path_race);

            EmbFile dyt_emb_race;

            if (!xv2fs->LoadFile(&dyt_emb_race, file_path_race))
                continue;

            if (dyt_emb_x2m->GetNumFiles() >= 3 && dyt_emb_race.GetNumFiles() >= 3)
            {
                bool continue_main_loop = false;

                for (int f = 0; f < 3; f++)
                {
                    if ((*dyt_emb_x2m)[f] != dyt_emb_race[f])
                    {
                        continue_main_loop = true;
                        break;
                    }
                }

                if (continue_main_loop)
                    continue;
            }
        }

        //DPRINTF("%s\n", file_path_race.c_str());

check_label:

        if (emd_buf_x2m) delete[] emd_buf_x2m;
        if (emd_buf_x2m_ph) delete[] emd_buf_x2m_ph;
        if (dyt_emb_x2m) delete dyt_emb_x2m;

        if (part_idx == PART_HAIR && i >= 300)
        {
            //DPRINTF("Model is %d\n", i);
            *part_out = AccesoryItem((uint32_t)i, race);

            if (*part_out == (uint32_t)-1)
            {
                DPRINTF("%s: hair is bigger than 300 (%Id) but cannot find accessory item. On partset %d\n", FUNCNAME, i, slot_entry.costume_index);
                return false;
            }

            *is_accessory_out = true;
        }
        else if (part_idx == PART_BUST)
        {
            *part_out = BustItem((uint32_t)i, race);

            // No clothes, not error
            /*if (*part_out == (uint32_t)-1)
            {
                DPRINTF("%s: found bust model at %Id, but not item. On partset %d\n", FUNCNAME, i, slot_entry.costume_index);
                return false;
            }*/
        }
        else if (part_idx == PART_PANTS)
        {
            *part_out = PantsItem((uint32_t)i, race);

            // No clothes, not error
            /*if (*part_out == (uint32_t)-1)
            {
                DPRINTF("%s: found pants model at %Id, but not item. On partset %d\n", FUNCNAME, i, slot_entry.costume_index);
                return false;
            }*/
        }
        else if (part_idx == PART_RIST)
        {
            *part_out = RistItem((uint32_t)i, race);

            // No clothes, not error
            /*
            if (*part_out == (uint32_t)-1)
            {
                DPRINTF("%s: found rist model at %Id, but not item. On partset %d\n", FUNCNAME, i, slot_entry.costume_index);
                return false;
            }*/
        }
        else if (part_idx == PART_BOOTS)
        {
            *part_out = BootsItem((uint32_t)i, race);

            // No clothes, not error
            /*if (*part_out == (uint32_t)-1)
            {
                DPRINTF("%s: found boots model at %Id, but not item. On partset %d\n", FUNCNAME, i, slot_entry.costume_index);
                return false;
            }*/
        }
        else
        {
            *part_out = (uint32_t)i;
        }

        colors_out[0] = 0xFFFFFFFF;
        colors_out[1] = 0xFFFFFFFF;
        colors_out[2] = 0xFFFFFFFF;
        colors_out[3] = 0xFFFFFFFF;

        if (part_idx == PART_FACE_BASE)
        {
            // Let's return skin color here
            if (!GetColor(x2m_bcs, x2m_part, "SKIN_", &colors_out[0]))
                GetColor(x2m_bcs, x2m_part, "SKIN_A_", &colors_out[0]);

            GetColor(x2m_bcs, x2m_part, "SKIN_B_", &colors_out[1]);
            GetColor(x2m_bcs, x2m_part, "SKIN_C_", &colors_out[2]);
            GetColor(x2m_bcs, x2m_part, "SKIN_D_", &colors_out[3]);
        }
        else if (part_idx == PART_FACE_FOREHEAD)
        {
            // Let's return paint colors here
            GetColor(x2m_bcs, x2m_part, "PAINT_A_", &colors_out[0]);
            GetColor(x2m_bcs, x2m_part, "PAINT_B_", &colors_out[1]);
            GetColor(x2m_bcs, x2m_part, "PAINT_C_", &colors_out[2]);
        }
        else if (part_idx == PART_FACE_EYE)
        {
            GetColor(x2m_bcs, x2m_part, "eye_", &colors_out[0]);
        }
        else if (part_idx == PART_HAIR)
        {
            GetColor(x2m_bcs, x2m_part, "HAIR_", &colors_out[0]);
        }
        else if (part_idx == PART_BUST)
        {
            GetColor(x2m_bcs, x2m_part, "CC00_BUST_", &colors_out[0]);
            GetColor(x2m_bcs, x2m_part, "CC01_BUST_", &colors_out[1]);
            GetColor(x2m_bcs, x2m_part, "CC02_BUST_", &colors_out[2]);
            GetColor(x2m_bcs, x2m_part, "CC03_BUST_", &colors_out[3]);
        }
        else if (part_idx == PART_PANTS)
        {
            GetColor(x2m_bcs, x2m_part, "CC00_PANTS_", &colors_out[0]);
            GetColor(x2m_bcs, x2m_part, "CC01_PANTS_", &colors_out[1]);
            GetColor(x2m_bcs, x2m_part, "CC02_PANTS_", &colors_out[2]);
            GetColor(x2m_bcs, x2m_part, "CC03_PANTS_", &colors_out[3]);
        }
        else if (part_idx == PART_RIST)
        {
            GetColor(x2m_bcs, x2m_part, "CC00_RIST_", &colors_out[0]);
            GetColor(x2m_bcs, x2m_part, "CC01_RIST_", &colors_out[1]);
            GetColor(x2m_bcs, x2m_part, "CC02_RIST_", &colors_out[2]);
            GetColor(x2m_bcs, x2m_part, "CC03_RIST_", &colors_out[3]);
        }
        else if (part_idx == PART_BOOTS)
        {
            GetColor(x2m_bcs, x2m_part, "CC00_BOOTS_", &colors_out[0]);
            GetColor(x2m_bcs, x2m_part, "CC01_BOOTS_", &colors_out[1]);
            GetColor(x2m_bcs, x2m_part, "CC02_BOOTS_", &colors_out[2]);
            GetColor(x2m_bcs, x2m_part, "CC03_BOOTS_", &colors_out[3]);
        }

        return true;
    }

    if (best_candidate >= 0)
    {
        i = (size_t)best_candidate;
        goto check_label;
    }

    if (!null_model)
    {
        uint8_t emd_md5[16];

        Utils::Md5(emd_buf_x2m, (uint32_t)emd_size_x2m, emd_md5);

        if (emd_buf_x2m)
        {
            delete[] emd_buf_x2m;
            emd_buf_x2m = nullptr;
        }

        if (part_idx == PART_PANTS)
        {
            for (const Md5Pair &pair : md5_pants)
            {
                if (memcmp(pair.md5, emd_md5, 16) == 0)
                {
                    i = pair.model;
                    goto check_label;
                }
            }
        }
        else if (part_idx == PART_BOOTS)
        {
            for (const Md5Pair &pair : md5_boots)
            {
                if (memcmp(pair.md5, emd_md5, 16) == 0)
                {
                    i = pair.model;
                    goto check_label;
                }
            }
        }

        if (part_idx >= PART_BUST && part_idx <= PART_BOOTS)
        {
            //DPRINTF("%d on part %d\n", x2m_part.model, part_idx);

            if (x2m_part.model >= 300 && x2m_part.model <= 999)
            {
                i = x2m_part.model;
                goto check_label;
            }
        }
    }

    DPRINTF("%s: Couldn't find model. Partset %d, part %d\n", FUNCNAME, slot_entry.costume_index, part_idx);
    return false;
}

bool X2m2Cac::FindAccessory(X2mFile *x2m, const BcsFile &race_bcs, uint32_t slot_idx, uint32_t *accessory)
{
    std::string race;

    if (!GetRace(x2m, race))
    {
        //DPRINTF("%s: Failed to get race.\n", FUNCNAME);
        return false;
    }

    if (race == "SYM")
        race = "HUM";
    else if (race == "SYF")
        race = "HUF";

    size_t bcs_size;
    uint8_t *bcs_buf = x2m->ReadFile(Utils::MakePathString(x2m->GetEntryName(), x2m->GetEntryName() + ".bcs"), &bcs_size);

    if (!bcs_buf)
    {
        //DPRINTF("%s: Cannot read bcs file.\n", FUNCNAME);
        return false;
    }

    BcsFile x2m_bcs;
    bool ret = x2m_bcs.Load(bcs_buf, bcs_size);
    delete[] bcs_buf;

    if (!ret)
    {
        //DPRINTF("%s: Cannot load bcs file.\n", FUNCNAME);
        return false;
    }

    const X2mSlotEntry &slot_entry = x2m->GetSlotEntry(slot_idx);
    const std::vector<BcsPartSet> &x2m_partsets = x2m_bcs.GetPartSets();

    if (slot_entry.costume_index >= x2m_partsets.size())
    {
        //DPRINTF("%s: costume_index %d doesn't exist in bcs.\n", FUNCNAME, slot_entry.costume_index);
        return false;
    }

    const BcsPartSet &x2m_partset = x2m_partsets[slot_entry.costume_index];

    if (!x2m_partset.valid)
    {
        //DPRINTF("%s: Partset %d is empty.\n", FUNCNAME, slot_entry.costume_index);
        return false;
    }

    const std::vector<BcsPartSet> &race_partsets = race_bcs.GetPartSets();

    for (int i = 0; i <= PART_BOOTS; i++)
    {
        const BcsPart &x2m_part = x2m_partset.parts[i];

        if (!x2m_part.valid || x2m_part.physics.size() == 0)
            continue;

        for (const BcsPhysics &x2m_ph : x2m_part.physics)
        {
            if (x2m_ph.unk_28[0].length() == 0)
                continue;

            std::string file_path_x2m = x2m_ph.unk_28[0] + ".emd";
            file_path_x2m = Utils::MakePathString(x2m->GetEntryName(), file_path_x2m);

            size_t emd_buf_size;
            uint8_t *emd_buf_x2m = x2m->ReadFile(file_path_x2m, &emd_buf_size);

            if (!emd_buf_x2m)
                continue;

            for (size_t j = 300; j < race_partsets.size(); j++)
            {
                const BcsPartSet &race_partset = race_partsets[j];
                if (!race_partset.valid)
                    continue;

                const BcsPart &race_part = race_partset.parts[i];
                if (!race_part.valid || race_part.physics.size() == 0)
                    continue;

                for (const BcsPhysics &race_ph : race_part.physics)
                {
                    if (race_ph.unk_28[0].length() == 0)
                        continue;

                    std::string file_path_race = race_ph.unk_28[0] + ".emd";
                    file_path_race = Utils::MakePathString("/data/chara/" + race, file_path_race);

                    size_t emd_size_race = xv2fs->GetFileSize(file_path_race);
                    if (emd_size_race != emd_buf_size)
                        continue;

                    uint8_t *emd_buf_race = xv2fs->ReadFile(file_path_race, &emd_size_race);
                    if (!emd_buf_race)
                        continue;

                    bool equal = (memcmp(emd_buf_x2m, emd_buf_race, emd_size_race) == 0);
                    delete[] emd_buf_race;
                    if (!equal)
                        continue;

                    *accessory = AccesoryItem((uint32_t)j, race);

                    if (*accessory != (uint32_t)-1)
                    {
                        delete[] emd_buf_x2m;
                        return true;
                    }
                }
            }

            if (emd_buf_x2m) delete[] emd_buf_x2m;
        }
    }

    return false;
}

uint32_t X2m2Cac::GetTalisman(X2mFile *x2m, uint32_t slot_idx)
{
    if (x2m->GetType() != X2mType::NEW_CHARACTER)
        return (uint32_t)-1;

    if (!x2m->HasPsc() || slot_idx >= x2m->GetNumSlotEntries())
        return (uint32_t)-1;

    const X2mSlotEntry &slot_entry = x2m->GetSlotEntry(slot_idx);
    uint32_t psc_idx = (uint32_t)slot_entry.costume_index;

    if (psc_idx >= x2m->GetNumSlotEntries())
        return (uint32_t)-1;

    return x2m->GetPscEntry(psc_idx).talisman;
}

struct CustomSkillParam
{
    uint8_t guid[16];
    uint32_t skill_out;

    CustomSkillParam()
    {
        skill_out = 0xFFFFFFFF;
    }
};

static bool custom_skill_visitor(const std::string &path, bool, void *custom_param)
{
    const std::string &name = Utils::GetFileNameString(path);

    if (Utils::ToLowerCase(name) != "x2m_skill.ini")
        return true;

    IniFile ini;

    if (!ini.LoadFromFile(path, false))
        return true;

    std::string guid_str;
    uint8_t guid[16];

    if (!ini.GetStringValue("General", "GUID", guid_str))
        return true;

    if (!Utils::String2GUID(guid, guid_str))
        return true;

    CustomSkillParam *param = (CustomSkillParam *)custom_param;

    if (memcmp(param->guid, guid, sizeof(guid)) != 0)
        return true;

    std::string dir = Utils::GetDirNameString(path);

    while (dir.back() == '/')
        dir.pop_back();

    dir = Utils::GetFileNameString(dir);
    size_t pos = dir.find('_');

    if (pos == 0 || pos == std::string::npos)
        return true;

    std::string skill_id = dir.substr(0, pos);
    param->skill_out = Utils::GetUnsigned(skill_id);

    return false;
}

uint32_t X2m2Cac::GetSkill(X2mFile *x2m, uint32_t slot_idx, uint32_t skill_idx, bool format2)
{
    if (x2m->GetType() != X2mType::NEW_CHARACTER || skill_idx >= 9)
        return (uint32_t)-1;

    if (slot_idx >= x2m->GetNumSkillSets())
        return (uint32_t)-1;

    const CusSkillSet &skill_set = x2m->GetSkillSet(slot_idx);
    uint32_t skill_id = skill_set.char_skills[skill_idx];

    std::string path;
    uint32_t start_id;

    if (skill_idx < 4)
    {
        path = "data/skill/SPA";
        start_id = CUS_SUPER_START;
    }
    else if (skill_idx < 6)
    {
        path = "data/skill/ULT";
        start_id = CUS_ULTIMATE_START;
    }
    else if (skill_idx < 7)
    {
        path = "data/skill/ESC";
        start_id = CUS_EVASIVE_START;
    }
    else if (skill_idx < 8)
    {
        path = "data/skill/BLA";
        start_id = CUS_BLAST_START;
    }
    else
    {
        path = "data/skill/MET";
        start_id = CUS_AWAKEN_START;
    }

    if (skill_id < X2M_SKILL_DEPENDS_BEGIN)
    {
        if (format2 && skill_id != (uint32_t)-1)
        {
            skill_id -= start_id;
        }

        return skill_id;
    }

    size_t custom_idx = skill_id - X2M_SKILL_DEPENDS_BEGIN;
    if (!x2m->HasCharaSkillDepends() || custom_idx >= x2m->GetNumCharaSkillDepends())
        return (uint32_t)-1;

    CustomSkillParam param;
    memcpy(param.guid, x2m->GetCharaSkillDepends(custom_idx).guid, sizeof(param.guid));

    Utils::VisitDirectory(Utils::MakePathString(xv2fs->GetLooseRoot(), path), true, false, true, custom_skill_visitor, &param);
    skill_id = param.skill_out;

    if (!format2 && skill_id != (uint32_t)-1)
    {
        skill_id += start_id;
    }

    return skill_id;
}

bool X2m2Cac::GetStats(X2mFile *x2m, uint32_t slot_idx, uint16_t *hea, uint16_t *ki, uint16_t *stm, uint16_t *atk, uint16_t *str, uint16_t *bla)
{
    std::string race;
    uint32_t race_cms;

    if (!GetRace(x2m, race))
    {
        //DPRINTF("%s: Failed to get race.\n", FUNCNAME);
        return false;
    }

    CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByName(race));
    race_cms = cms->id;

    if (slot_idx >= x2m->GetNumSlotEntries())
        return false;

    if (!x2m->UsesBodyShape())
        return false;

    const X2mSlotEntry &slot_entry = x2m->GetSlotEntry(slot_idx);
    uint32_t psc_idx = (uint32_t)slot_entry.costume_index;

    if (psc_idx >= x2m->GetNumSlotEntries())
        return false;

    const PscSpecEntry &psc = x2m->GetPscEntry(psc_idx);

    PscSpecEntry *orig_psc = game_psc->FindSpec(race_cms, x2m->GetBodyShape(), 0);
    if (!orig_psc)
    {
        DPRINTF("%s: Cannot find psc entry for race 0x%x and body shape %d.\n", FUNCNAME, race_cms, x2m->GetBodyShape());
        return false;
    }

    *hea = game_psa->GetHeaPoints(psc.health-orig_psc->health);
    if (*hea == 0xFFFF) *hea = 0;

    *ki = game_psa->GetKiPoints(psc.ki-orig_psc->ki);
    if (*ki == 0xFFFF) *ki = 0;

    *stm = game_psa->GetStaPoints(psc.stamina-orig_psc->stamina);
    if (*stm == 0xFFFF) *stm = 0;

    *atk = game_psa->GetAtkPoints(psc.basic_attack-orig_psc->basic_attack);
    if (*atk == 0xFFFF) *atk = 0;

    *str = game_psa->GetStrPoints(psc.strike_attack-orig_psc->strike_attack);
    if (*str == 0xFFFF) *str = 0;

    *bla = game_psa->GetBlaPoints(psc.ki_blast_super-orig_psc->ki_blast_super);
    if (*bla == 0xFFFF) *bla = 0;

    return true;
}

uint32_t X2m2Cac::GetRace(X2mFile *x2m)
{
    std::string race;

    if (!GetRace(x2m, race))
        return (uint32_t)-1;

    if (race == "HUM")
        return CAC_HUM;
    else if (race == "HUF")
        return CAC_HUF;
    else if (race == "SYM")
        return CAC_SYM;
    else if (race == "SYF")
        return CAC_SYF;
    else if (race == "NMC")
        return CAC_NMC;
    else if (race == "FRI")
        return CAC_FRI;
    else if (race == "MAM")
        return CAC_MAM;
    else if (race == "MAF")
        return CAC_MAF;

    return (uint32_t)-1;
}



