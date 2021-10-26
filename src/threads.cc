// Wayventure, a complex old-school dungeon adventure.
// Copyright (C) 2021 Zhi Ping Ooi
//
// This file is part of Wayventure.
//
// Wayventure is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "headers/threads.h"
#include "headers/generate.h"
#include <atomic>
#include <thread>

void refresh_lootbox(const std::atomic<bool> &terminate, std::atomic<bool> &require_processing, Dungeon &dungeon_data){
    if(!dungeon_data.loot_data.empty()){
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
    while(!terminate.load(std::memory_order_acquire)){
        dungeon_data.loot_data.clear();
        int amount_of_lootboxes=generate_random_number(20,50);
        for(int i = 0; i<amount_of_lootboxes; i++){
            LootData temp_data;
            temp_data.id=i;
            temp_data.dungeon_position={generate_random_number(1,5),generate_random_number(1,5),generate_random_number(1,5)};
            temp_data.room_position={generate_random_number(1,78),generate_random_number(1,48)};
            dungeon_data.loot_data.push_back(temp_data);
        }
        require_processing.store(true, std::memory_order_release);
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
}

void job_thread(Miner &miner, Archaeologist &archaeologist, const std::atomic<bool> &terminate){
    while(!terminate.load(std::memory_order_acquire)){
        if(miner.job.has_active_job&&miner.job.is_job_finished()){
            miner.loot.mysterious_piece+=(((20*miner.job.number_of_miners)+generate_random_number(0, 2*miner.job.number_of_miners))*miner.job.loot_multiplier)*((500+miner.skill_level)/500);
            miner.loot.mysterious_artifact+=(((4*miner.job.number_of_miners)+generate_random_number(0, miner.job.number_of_miners))*miner.job.loot_multiplier)*((500+miner.skill_level)/500);
            miner.skill_level+=(miner.job.number_of_miners+generate_random_number(0, miner.job.number_of_miners))/pow(1.02,miner.skill_level);
            if(miner.skill_level>250.0){
                miner.skill_level=250.0;
            }
            miner.job=Miner::Job_Details();
        }
        if(archaeologist.job.has_active_job&&archaeologist.job.is_job_finished()){
            // Material 0, 5, 20, 50, 100, 225
            for(int i=0; i<archaeologist.job.decryption_amount.mysterious_piece; i++){
                if(archaeologist.skill_level<250.0){
                    archaeologist.skill_level+=(1/pow(1.04,miner.skill_level));
                }
                if(return_chance(static_cast<int>(75+((miner.skill_level)/10)))){
                    continue;
                }
                int chance=generate_random_number(0, miner.skill_level);
                if(chance<5){
                    archaeologist.loot_storage.materials.common++;
                }
                else if(chance<20){
                    archaeologist.loot_storage.materials.uncommon++;
                }
                else if(chance<50){
                    archaeologist.loot_storage.materials.rare++;
                }
                else if(chance<100){
                    archaeologist.loot_storage.materials.epic++;
                }
                else if(chance<225){
                    archaeologist.loot_storage.materials.legendary++;
                }
                else{
                    archaeologist.loot_storage.materials.artifact++;
                }

            }
            // Cores 0, 75, 150, 225
            for(int i=0; i<archaeologist.job.decryption_amount.mysterious_artifact; i++){
                if(archaeologist.skill_level<250.0){
                    archaeologist.skill_level+=(4/pow(1.04,miner.skill_level));
                }
                if(return_chance(static_cast<int>(50+((miner.skill_level)/5)))){
                    continue;
                }
                int chance=generate_random_number(0, miner.skill_level);
                if(chance<75){
                    archaeologist.loot_storage.cores.crystal_core++;
                }
                else if(chance<150){
                    archaeologist.loot_storage.cores.crystallium++;
                }
                else if(chance<225){
                    archaeologist.loot_storage.cores.mysterious_shard++;
                }
                else{
                    archaeologist.loot_storage.cores.ancient_core++;
                }

            }
            archaeologist.job=Archaeologist::Job_Details();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return;
}
