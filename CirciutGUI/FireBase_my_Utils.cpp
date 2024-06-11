
#include "LOG.hpp"
#include "FireBase_GUI.hpp"

#include "FireBase_my_Utils.hpp"

namespace fb
 {

    firebase::App* fireBase_app = nullptr;
    firebase::database::Database* database = nullptr;
    //firebase::Variant v;

    BotsData LastestBotData;
    void LoadLastestBotData()
    {
        if (!database) return;

        if (1 || LastestBotData.isInputDone == true)
        {
            LastestBotData.isInputDone = false;
            LastestBotData.result.Release();

            auto node_ref = database->GetReference("Blocks");
            LastestBotData.result = node_ref.Child("Leatest").GetValue();
        }
    }
    void WriteLastestBotData()
    {
        /*if (!database)*/ return;

        if (1 || LastestBotData.isOutputDone == true)
        {
            using  namespace GUI;

            LastestBotData.isOutputDone = false;
            LastestBotData.writeResult.Release();

            firebase::Variant variant;
            std::vector<firebase::Variant> blockSetData = { 0,0,0,0,0 };

            {
                if ((0 <= BlockPicked.x && BlockPicked.x < 4) ||
                    (0 <= BlockPicked.y /*&& BlockPicked.y < GreenBlocks.size()*/))
                {
                    int i = BlockPicked.y;
                    std::vector<Block>* vec = nullptr;

                    if (BlockPicked.x == 0)      vec = &GreenBlocks;
                    else if (BlockPicked.x == 1) vec = &RedBlocks;
                    else if (BlockPicked.x == 2) vec = &BlueBlocks;
                    else if (BlockPicked.x == 3) vec = &YellowBlocks;

                    if (vec && 0 <= i && i < (*vec).size())
                    {
                        auto& block = (*vec)[i];

                        blockSetData[0].set_int64_value(BlockPicked.x);
                        blockSetData[1].set_int64_value(BlockPicked.y);
                        blockSetData[2].set_int64_value(block.getRot());
                        blockSetData[3].set_int64_value(block.getPos().x);
                        blockSetData[4].set_int64_value(block.getPos().y);
                    }
                }
            }

            variant.set_vector(blockSetData);

            auto node_ref = database->GetReference("Blocks");
            LastestBotData.writeResult = node_ref.Child("Leatest").SetValue(variant);
        }


        //void WriteLastestBotData()
    //{
    //    if (!database) return;
    //
    //    if (LastestBotData.isOutputDone == true)
    //    {
    //        using  namespace GUI;
    //
    //        LastestBotData.isOutputDone = false;
    //
    //        firebase::Variant variant;
    //        std::vector<firebase::Variant> blockSetData = { 0,0,0,0,0 };
    //
    //        {
    //            if ((0 <= BlockPicked.x && BlockPicked.x < 4) ||
    //                (0 <= BlockPicked.y /*&& BlockPicked.y < GreenBlocks.size()*/))
    //            {
    //                int i = BlockPicked.y;
    //                std::vector<Block>* vec = nullptr;
    //
    //                if (BlockPicked.x == 0)      vec = &GreenBlocks;
    //                else if (BlockPicked.x == 1) vec = &RedBlocks;
    //                else if (BlockPicked.x == 2) vec = &BlueBlocks;
    //                else if (BlockPicked.x == 3) vec = &YellowBlocks;
    //
    //                if (vec && 0 <= i && i < (*vec).size())
    //                {
    //                    auto& block = (*vec)[i];
    //
    //                    blockSetData[0].set_int64_value(BlockPicked.x);
    //                    blockSetData[1].set_int64_value(BlockPicked.y);
    //                    blockSetData[2].set_int64_value(block.getRot());
    //                    blockSetData[3].set_int64_value(block.getPos().x);
    //                    blockSetData[4].set_int64_value(block.getPos().y);
    //                }
    //            }
    //        }
    //
    //        variant.set_vector(blockSetData);
    //
    //        auto node_ref = database->GetReference("Blocks");
    //        LastestBotData.writeResult = node_ref.Child("Leatest").SetValue(variant);
    //    }
    //}
    }
    void UpdateLatestBotResults()
    {
        if (!database) return;

        if (LastestBotData.writeResult.status() != firebase::FutureStatus::kFutureStatusPending)
        {
            LastestBotData.isOutputDone = true;
            LastestBotData.writeResult.Release();
        }


        if ((!LastestBotData.isInputDone && LastestBotData.result.status() != firebase::FutureStatus::kFutureStatusPending) ||
            (LastestBotData.timer.repeatEvery(100)))
        {
            using namespace GUI;

            if (LastestBotData.result.status() == firebase::FutureStatus::kFutureStatusComplete)
            {
                LOG("\nLastestBotData - Complete");

                auto variant = LastestBotData.result.result()->value();

                if (variant.is_vector())
                {
                    LOG("\nLastestBotData\t");

                    int64_t input[5] = { -1,-1,0,0,0 };

                    LOG("(");
                    for (int i = 0; i < 5 && i < variant.vector().size(); i++)
                    {
                        LOG(variant.vector()[i].int64_value() << ",");

                        input[i] = variant.vector()[i].int64_value();
                    }
                    LOG(") ");


                    {
                        if ((0 <= input[0] && input[0] < 4) ||
                            (0 <= input[1] /*&& BlockPicked.y < GreenBlocks.size()*/))
                        {
                            if ((input[0] == BlockPicked.x) ||
                                (input[1] == BlockPicked.y))
                                return;


                            int i = input[1];
                            std::vector<Block>* vec = nullptr;

                                 if (input[0] == 0) vec = &GreenBlocks;
                            else if (input[0] == 1) vec = &RedBlocks;
                            else if (input[0] == 2) vec = &BlueBlocks;
                            else if (input[0] == 3) vec = &YellowBlocks;

                            if (vec && 0 <= i && i < (*vec).size())
                            {
                                auto& block = (*vec)[i];

                                block.setRot(input[2]);
                                block.setPos(sf::Vector2f(input[3], input[4]));
                            }
                        }
                    }
                }
                else LOG("\n LastestBotData Variant is not a Vector");
            }
            else LOG("\nLastestBotData - Invalid");

            LastestBotData.isInputDone = true;
            LastestBotData.result.Release();
        }
        else LOG("\nLastestBotData - Pending" << LastestBotData.isInputDone);
    
    }
    
    BotsData allBlockData;
    //BotsData greenData;
    //BotsData redData;
    //BotsData blueData;
    //BotsData yellowData;
    void LoadAllBloackSets()
    {
        static Timer t_load;

        if (t_load.repeatEvery(15))
        {
            fb::LoadBlockSet(10);
            //fb::LoadBlockSet(0);
            //fb::LoadBlockSet(1);
            //fb::LoadBlockSet(2);
            //fb::LoadBlockSet(3);
        }
    }
    void WriteAllBloackSets()
    {
        static Timer t_send;

        if (t_send.repeatEvery(14))
        {
            fb::WriteBlockSet(10);
            //fb::WriteBlockSet(0);
            //fb::WriteBlockSet(1);
            //fb::WriteBlockSet(2);
            //fb::WriteBlockSet(3);
        }
    }
    void UpdateWriteResults()
    {
        if (allBlockData.writeResult.status() != firebase::FutureStatus::kFutureStatusPending) allBlockData.isOutputDone = true;
        //if (greenData .writeResult.status() != firebase::FutureStatus::kFutureStatusPending) greenData .isOutputDone = true;
        //if (redData   .writeResult.status() != firebase::FutureStatus::kFutureStatusPending) redData   .isOutputDone = true;
        //if (blueData  .writeResult.status() != firebase::FutureStatus::kFutureStatusPending) blueData  .isOutputDone = true;
        //if (yellowData.writeResult.status() != firebase::FutureStatus::kFutureStatusPending) yellowData.isOutputDone = true;
    }
    void UpdateLoadResults()
    {
        using GUI::GreenBlocks, GUI::RedBlocks, GUI::BlueBlocks, GUI::YellowBlocks;

        // All
        {
            if ((!allBlockData.isInputDone && allBlockData.result.status() != firebase::FutureStatus::kFutureStatusPending) ||
                (allBlockData.timer.repeatEvery(150)))
            {
                if (allBlockData.result.status() == firebase::FutureStatus::kFutureStatusComplete)
                {
                    auto variant = allBlockData.result.result()->value();
                    
                    if (variant.is_vector())
                    {
                        LOG("\nAll\t");

                        int blockNo = 0;
                        for (auto& num : variant.vector())
                        {
                            if (52 <= blockNo)
                            {
                                LOG("\n 52 <= BlockNo");
                                break;
                            }

                            if (num.is_int64())
                            {
                                int64_t en = num.int64_value();
                                int64_t a = 0, b = 0;

                                a = (int32_t)((4294967295) & en);
                                b = (int32_t)(((4294967295 << 32) & en) >> 32);

                                //if (((1 << 31) & en))
                                //{
                                //    a -= 1;
                                //    a ^= 4294967295;
                                //    a *= -1;
                                //}

                                LOG(blockNo << " ");
                                if ( 0 <= blockNo && blockNo < 13) GreenBlocks [blockNo++ % 13].setSerializedPacket(a); else
                                if (13 <= blockNo && blockNo < 26) RedBlocks   [blockNo++ % 13].setSerializedPacket(a); else
                                if (26 <= blockNo && blockNo < 39) BlueBlocks  [blockNo++ % 13].setSerializedPacket(a); else
                                if (39 <= blockNo && blockNo < 52) YellowBlocks[blockNo++ % 13].setSerializedPacket(a);

                                if ( 0 <= blockNo && blockNo < 13) GreenBlocks [blockNo++ % 13].setSerializedPacket(b); else
                                if (13 <= blockNo && blockNo < 26) RedBlocks   [blockNo++ % 13].setSerializedPacket(b); else
                                if (26 <= blockNo && blockNo < 39) BlueBlocks  [blockNo++ % 13].setSerializedPacket(b); else
                                if (39 <= blockNo && blockNo < 52) YellowBlocks[blockNo++ % 13].setSerializedPacket(b);

                            }
                            else LOG("\n block is not an int64");
                        }
                    }
                    else LOG("\n Variant is not a Vector");
                }
                else LOG("\nAll - Invalid");
                
                allBlockData.isInputDone = true;
                allBlockData.result.Release();
            }
            else LOG("\nAll - Pending" << allBlockData.isInputDone);
        }

        // Green
        /*{
            if ((!greenData.isInputDone && greenData.result.status() != firebase::FutureStatus::kFutureStatusPending) ||
                (greenData.timer.repeatEvery(15)))
            {
                if (greenData.result.status() == firebase::FutureStatus::kFutureStatusComplete)
                {
                    auto variant = greenData.result.result()->value();

                    if (variant.is_vector())
                    {
                        LOG("\nGreen\t");

                        int blockNo = 0;
                        for (auto& block : variant.vector())
                        {
                            if (GreenBlocks.size() <= blockNo)
                            {
                                LOG("\n GreenBlocks.size() <= BlockNo");
                                break;
                            }

                            if (block.is_int64())
                            {
                                GreenBlocks[blockNo].setSerializedPacket(block.int64_value());
                                blockNo++;
                            }
                            else LOG("\n block is not an int64");
                        }
                    }
                    else LOG("\n Variant is not a Vector");
                }
                else LOG("\nGreen - Invalid");

                greenData.isInputDone = true;
                greenData.result.Release();
            }
            else LOG("\nGreen - Pending" << greenData.isInputDone);
        }*/

        // Red
        /*{
            if ((!redData.isInputDone && redData.result.status() != firebase::FutureStatus::kFutureStatusPending) ||
                (redData.timer.repeatEvery(15)))
            {
                if (redData.result.status() == firebase::FutureStatus::kFutureStatusComplete)
                {
                    auto variant = redData.result.result()->value();

                    if (variant.is_vector())
                    {
                        LOG("\nRed\t");

                        int blockNo = 0;
                        for (auto& block : variant.vector())
                        {
                            if (RedBlocks.size() <= blockNo)
                            {
                                LOG("\n RedBlocks.size() <= BlockNo");
                                break;
                            }

                            if (block.is_int64())
                            {
                                RedBlocks[blockNo].setSerializedPacket(block.int64_value());
                                blockNo++;
                            }
                            else LOG("\n block is not an int64");
                        }
                    }
                    else LOG("\n Variant is not a Vector");
                }
                else LOG("\nRed - Invalid");

                redData.isInputDone = true;
                redData.result.Release();
            }
            else LOG("\nRed - Pending" << redData.isInputDone);
        }*/

        // Blue
        /*{
            if ((!blueData.isInputDone && blueData.result.status() != firebase::FutureStatus::kFutureStatusPending) ||
                (blueData.timer.repeatEvery(15)))
            {
                if (blueData.result.status() == firebase::FutureStatus::kFutureStatusComplete)
                {
                    auto variant = blueData.result.result()->value();

                    if (variant.is_vector())
                    {
                        LOG("\nBlue\t");

                        int blockNo = 0;
                        for (auto& block : variant.vector())
                        {
                            if (BlueBlocks.size() <= blockNo)
                            {
                                LOG("\n BlueBlocks.size() <= BlockNo");
                                break;
                            }

                            if (block.is_int64())
                            {
                                BlueBlocks[blockNo].setSerializedPacket(block.int64_value());
                                blockNo++;
                            }
                            else LOG("\n block is not an int64");
                        }
                    }
                    else LOG("\n Variant is not a Vector");
                }
                else LOG("\nBlue - Invalid");

                blueData.isInputDone = true;
                blueData.result.Release();
            }
            else LOG("\nBlue - Pending" << blueData.isInputDone);
        }*/

        // Yellow
        /*{
            if ((!yellowData.isInputDone && yellowData.result.status() != firebase::FutureStatus::kFutureStatusPending) ||
                (yellowData.timer.repeatEvery(15)))
            {
                if (yellowData.result.status() == firebase::FutureStatus::kFutureStatusComplete)
                {
                    auto variant = yellowData.result.result()->value();

                    if (variant.is_vector())
                    {
                        LOG("\nYellow\t");

                        int blockNo = 0;
                        for (auto& block : variant.vector())
                        {
                            if (YellowBlocks.size() <= blockNo)
                            {
                                LOG("\n YellowBlocks.size() <= BlockNo");
                                break;
                            }

                            if (block.is_int64())
                            {
                                YellowBlocks[blockNo].setSerializedPacket(block.int64_value());
                                blockNo++;
                            }
                            else LOG("\n block is not an int64");
                        }
                    }
                    else LOG("\n Variant is not a Vector");
                }
                else LOG("\nYellow - Invalid");

                yellowData.isInputDone = true;
                yellowData.result.Release();
            }
            else LOG("\nYellow - Pending" << yellowData.isInputDone);
        }*/
    }

    

    void InitializeFireBase() {
        firebase::AppOptions options; {
            options.set_api_key("AIzaSyCmnrOkK_9qZtF0fWRrnofqZ3b9KI_rMyc");
            options.set_app_id("105740420520");
            options.set_project_id("app1-d3365");
            options.set_messaging_sender_id("1:105740420520:web:bc6d0d8562e9cffd32c99d");
            options.set_database_url("https://app1-d3365-default-rtdb.asia-southeast1.firebasedatabase.app");
            options.set_storage_bucket("app1-d3365.appspot.com");
        }
        fireBase_app = firebase::App::Create(options);
        LOG("\n(+) FireBase is Set");
        database = firebase::database::Database::GetInstance(fireBase_app);
        LOG("\n(+) Database is Set");


        /*auth_my = firebase::auth::Auth::GetAuth(fireBase_app);
        auth_my->SignOut();*/
    }

    void Log(const firebase::Variant& variant, unsigned long indentLevel)
    {
        if (variant.is_int64()) {
            int value = variant.int64_value();
            LOG_TAB(indentLevel);
            LOG("int64: " << value << "\n");
        }
        else if (variant.is_double()) {
            double value = variant.double_value();
            LOG_TAB(indentLevel);
            LOG("double: " << value << "\n");
        }
        else if (variant.is_string()) {
            const std::string& value = variant.string_value();
            LOG_TAB(indentLevel);
            LOG("string: " << value << "\n");
        }
        else if (variant.is_bool()) {
            bool value = variant.bool_value();
            LOG_TAB(indentLevel);
            LOG("bool: " << value << "\n");
        }
        else if (variant.is_map()) {
            const std::map<firebase::Variant, firebase::Variant>& value = variant.map();

            LOG_TAB(indentLevel); LOG("Map(" << value.size() << "):\n");
            for (const auto& [key, val] : value)
            {
                LOG_TAB(indentLevel + 1); LOG("key:\n"); Log(key, indentLevel + 1);
                LOG_TAB(indentLevel + 1); LOG("val:\n"); Log(val, indentLevel + 1);
                LOG("\n")
            }
        }
        else if (variant.is_vector()) {
            const std::vector<firebase::Variant>& value = variant.vector();

            LOG_TAB(indentLevel);
            LOG("Vector(" << value.size() << "):\n");
            for (const auto& v : value)
                Log(v, indentLevel + 1);
        }
        else {
            // Handle unsupported or unexpected data type
            LOG_TAB(indentLevel);
            LOG("Invalid Type: " << firebase::Variant::TypeName(variant.type()));
        }
    }

    void DeleteNode(const std::string& node) {
        auto root_ref = database->GetReference().Child(node);
        firebase::Variant v;
        //root_ref.SetValue(v);
    }

    void ReadNode(const std::string& node_name)
    {
        if (!database) return;

        LOG("\n\nNode (" << node_name << "):\n");

        firebase::database::DatabaseReference node_ref = database->GetReference("Blocks").Child(node_name.c_str());

        node_ref.GetValue().OnCompletion([](const firebase::Future<firebase::database::DataSnapshot>& result) {
            if (result.status() == firebase::kFutureStatusComplete) {
                const firebase::Variant& variant = result.result()->value();
                Log(variant);
            }
            });
    }

    void WriteBlockSet(int BlockSet)
    {
        if (!database) return;

        if (BlockSet == 10) if (0 && allBlockData.isOutputDone == false) return; else { allBlockData.isOutputDone = false; allBlockData.writeResult.Release(); } //else
        //if (BlockSet == 0) if (0 && greenData .isOutputDone == false) return; else { greenData .isOutputDone = false; greenData .writeResult.Release(); } //else
        //if (BlockSet == 1) if (0 && redData   .isOutputDone == false) return; else { redData   .isOutputDone = false; redData   .writeResult.Release(); } //else
        //if (BlockSet == 2) if (0 && blueData  .isOutputDone == false) return; else { blueData  .isOutputDone = false; blueData  .writeResult.Release(); } //else
        //if (BlockSet == 3) if (0 && yellowData.isOutputDone == false) return; else { yellowData.isOutputDone = false; yellowData.writeResult.Release(); } //else
        //if (BlockSet < 0 || 3 < BlockSet) return;

        LOG("\nwrite " << BlockSet)


        firebase::Variant variant;
        //std::map<firebase::Variant, firebase::Variant> blockSetData;
        std::vector<firebase::Variant> blockSetData;
        
        for (int i = 0; i < 52;)
        {
            int64_t en = 0;
            int64_t a = 0, b = 0;

            if ( 0 <= i && i < 13) a = GUI::GreenBlocks [i++ % 13].getSerializedPacket(); else
            if (13 <= i && i < 26) a = GUI::RedBlocks   [i++ % 13].getSerializedPacket(); else
            if (26 <= i && i < 39) a = GUI::BlueBlocks  [i++ % 13].getSerializedPacket(); else
            if (39 <= i && i < 52) a = GUI::YellowBlocks[i++ % 13].getSerializedPacket();

            if ( 0 <= i && i < 13) b = GUI::GreenBlocks [i++ % 13].getSerializedPacket(); else
            if (13 <= i && i < 26) b = GUI::RedBlocks   [i++ % 13].getSerializedPacket(); else
            if (26 <= i && i < 39) b = GUI::BlueBlocks  [i++ % 13].getSerializedPacket(); else
            if (39 <= i && i < 52) b = GUI::YellowBlocks[i++ % 13].getSerializedPacket();

            en |= (4294967295 & a) << 0;
            en |= (4294967295 & b) << 32;

            blockSetData.emplace_back(en);
        }

        /*for (auto& [key, val] : blockSetData)
        {
            if (BlockSet == 0) val.set_int64_value(GUI::GreenBlocks [key.int64_value()].getSerializedPacket());
            if (BlockSet == 1) val.set_int64_value(GUI::RedBlocks   [key.int64_value()].getSerializedPacket());
            if (BlockSet == 2) val.set_int64_value(GUI::BlueBlocks  [key.int64_value()].getSerializedPacket());
            if (BlockSet == 3) val.set_int64_value(GUI::YellowBlocks[key.int64_value()].getSerializedPacket());
        }*/

        //variant.set_map(blockSetData);

        variant.set_vector(blockSetData);
        
        auto node_ref = database->GetReference("BlocksNew");
        if (BlockSet == 10) allBlockData.writeResult = node_ref.Child("AllBlocks").SetValue(variant);
        //if (BlockSet == 0) greenData .writeResult = node_ref.Child("Green") .SetValue(variant);
        //if (BlockSet == 1) redData   .writeResult = node_ref.Child("Red")   .SetValue(variant);
        //if (BlockSet == 2) blueData  .writeResult = node_ref.Child("Blue")  .SetValue(variant);
        //if (BlockSet == 3) yellowData.writeResult = node_ref.Child("Yellow").SetValue(variant);
    }

    void LoadBlockSet(int BlockSet) {

        if (!database) return;


        if (BlockSet == 10) if (allBlockData.isInputDone == false) return; else { allBlockData.isInputDone = false; allBlockData.result.Release(); }//else
        //if (BlockSet == 0) if (greenData .isInputDone == false) return; else { greenData .isInputDone = false; greenData .result.Release(); }//else
        //if (BlockSet == 1) if (redData   .isInputDone == false) return; else { redData   .isInputDone = false; redData   .result.Release(); }//else
        //if (BlockSet == 2) if (blueData  .isInputDone == false) return; else { blueData  .isInputDone = false; blueData  .result.Release(); }//else
        //if (BlockSet == 3) if (yellowData.isInputDone == false) return; else { yellowData.isInputDone = false; yellowData.result.Release(); }//else
        //return;

        //bot.data.clear();
         

        auto node_ref = database->GetReference("BlocksNew");
        if (BlockSet == 10) allBlockData.result = node_ref.Child("AllBlocks").GetValue();
        //if (BlockSet == 0) greenData .result = node_ref.Child("Green").GetValue();
        //if (BlockSet == 1) redData   .result = node_ref.Child("Red").GetValue();
        //if (BlockSet == 2) blueData  .result = node_ref.Child("Blue").GetValue();
        //if (BlockSet == 3) yellowData.result = node_ref.Child("Yellow").GetValue();
    }

    //firebase::auth::Auth* auth_my;
    /*void authh() {

        firebase::Future<firebase::auth::AuthResult> result =
            auth_my->CreateUserWithEmailAndPassword("cattred@gmail.com", "kerosine");
    }*/
}