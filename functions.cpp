#include <iostream>

#include "functions.hpp"
#include "handleFile.hpp"

struct ProcessControlBlock* PCB[PCBTOTAL];
struct Resource* RCB[RCBTOTAL];
struct LL* RL[RLTOTAL];

std::queue<int> processQ; // queue of next process number to generate

int currentPCB;

void release(){
    for(int i = 0; i < PCBTOTAL; i++){
        if(PCB[i] != NULL) delete PCB[i];
    }
    for(int i = 0; i < RCBTOTAL; i++){
        if(RCB[i] != NULL) delete RCB[i];
    }
    for(int i = 0; i < RLTOTAL; i++){
        if(RL[i] != NULL) delete RL[i];
    }
}

void init(){
    release();

    PCB[0] = new ProcessControlBlock(-1, 0);
    for(int i = 1; i < PCBTOTAL; i++){
        PCB[i] = NULL;
    }

    RL[0] = new LL(0);
    for(int i = 1; i < RLTOTAL; i++){
        RL[i] = NULL;
    }

    RCB[0] = new Resource(1);
    RCB[1] = new Resource(1);
    RCB[2] = new Resource(2);
    RCB[3] = new Resource(3);

    while(!processQ.empty()){
        processQ.pop();
    }
    for(int i = 1; i < PCBTOTAL; i++){
        processQ.push(i);
    }

    currentPCB = 0;
    newline();
    put(currentPCB);
}

void create(int priorityLevel){
    if(priorityLevel < 0 || priorityLevel >= RLTOTAL){
        std::cout << "*--Invalid Priority level, expect [0," << RLTOTAL-1 << 
            "] but got " << priorityLevel << " instead" << std::endl;
        put(-1);
        return;
    }
    if(processQ.empty()){
        std::cout << "*--No avaliable slot for new queue" << std::endl;
        put(-1);
        return;
    }
    int n = processQ.front();
    processQ.pop();
    PCB[n] = PCB[currentPCB]->createChild(n, priorityLevel);
    addToReadyList(n, priorityLevel);
    scheduler();
    put(currentPCB);
}

void freePCBResource(int pcbNum){
    struct LLResource* resource = PCB[pcbNum]->resources;
    struct LLResource* waitlistItem;
    // if(resource == NULL) return;
    while(resource != NULL){
        RCB[resource->num]->restore(resource->amount);
        waitlistItem = RCB[resource->num]->waitlist;
        while(waitlistItem != NULL && RCB[resource->num]->avaliable > 0){
            if(waitlistItem->amount <= RCB[resource->num]->avaliable){
                addToReadyList(waitlistItem->num, PCB[waitlistItem->num]->priority);
                PCB[waitlistItem->num]->appendResource(resource->num, waitlistItem->amount);
                RCB[resource->num]->removeFromWaitlist(waitlistItem);
            }
            else{
                break;
            }
            waitlistItem = waitlistItem->next;
        }

        resource = resource->next;
    }
    // RCB[]->restore(amount);
    // for each resource, 
    //      loop through waitlist of RCB and free item as needed;

}
// Helper for "void destroy(int childNum)"
void destroyPCB(int pcbNum){ 
    // Delete this PCB and all its children
    struct LL* children = PCB[pcbNum]->childs;
    freePCBResource(pcbNum);
    while(children != NULL){
        destroyPCB(children->value);
        children = children->next;
    }
    delete PCB[pcbNum];
    PCB[pcbNum] = NULL;
    processQ.push(pcbNum);

    std::string msg = removeFromReadyList(pcbNum);
    if(msg.size() != 0){ // Process not in ReadyList
        // Process in waitlist of some resource;
        bool removed = false;
        for(int i = 0; i < RCBTOTAL; i++){
            removed = RCB[i]->removeFromWaitlist(pcbNum);
            if(removed) break;
        }
    }
}

// Helper for "void destroy(int childNum)"
bool destroyDescendants(int pcbNum, int target){
    struct LL* children = PCB[pcbNum]->childs;
    if(children == NULL) return false;

    // Destroy direct child if found
    if(children->value == target){
        PCB[pcbNum]->childs = children->next;
        children->next = NULL;
        delete children;
        return true;
    }
    while(children->next != NULL){
        if(children->value == target){
            struct LL* s = children->next;
            children->next = s->next;
            s->next = NULL;
            delete s;
            return true;
        }
        children = children->next;
    }

    // Child not found
    // Recursive call to delete descendants furthur down the tree;
    children = PCB[pcbNum]->childs;
    while(children != NULL){
        if(destroyDescendants(children->value, target)) return true;
        children = children->next;
    }
    return false;
}

void destroy(int childNum){
    if(childNum <= 0 || childNum >= PCBTOTAL){ // Process 0 can not be destroyed
        std::cout << "*--Invalid child number, expect [0," << PCBTOTAL-1 << 
            "] but got " << childNum << " instead" << std::endl;
        put(-1);
        return;
    }
    if(PCB[childNum] == NULL){
        std::cout << "*--Invalid child number, child " <<
            childNum << " is NULL" << std::endl;
        put(-1);
        return;
    }
    if(childNum == currentPCB){
        destroyPCB(childNum);
        scheduler();
        put(currentPCB);
        return;
    }
    if(destroyDescendants(currentPCB, childNum)){
        destroyPCB(childNum);
        scheduler();
    }
    else{
        std::cout << "*--Invalid child number, child " <<
            childNum << " is not descendants of this process" << std::endl;
        put(-1);
        return;
    }
    put(currentPCB);
};

void request(int resourceNum, int amount){
    if(resourceNum < 0 || resourceNum >= RCBTOTAL){
        std::cout << "*--Invalid resource number, expect [0," << RCBTOTAL-1 << 
            "] but got " << resourceNum << " instead" << std::endl;
        put(-1);
        return;
    }
    if(amount <= 0){
        std::cout << "*--Invalid resource amount, expect >= 1, but got " <<
        amount << " instead" << std::endl;
        put(-1);
        return;
    }

    // Find if process holding current resource
    struct LLResource* holding = PCB[currentPCB]->resources;
    while(holding != NULL){
        if(holding->num == resourceNum){
            break;
        }
        holding = holding->next;
    }
    
    if(RCB[resourceNum]->waitlist != NULL){
        std::string msg;
        RCB[resourceNum]->addToWaitlist(currentPCB, amount);
        msg = removeFromReadyList(currentPCB); // call reschedule inside this funciton;
        if(msg.size() != 0){
            std::cout << msg << std::endl;
        }
    }
    else if(amount <= RCB[resourceNum]->avaliable){
        RCB[resourceNum]->avaliable -= amount;
        if(holding == NULL){
            PCB[currentPCB]->appendResource(resourceNum, amount);
        }
        else{
            holding->amount += amount;
        }
    }
    else{
        if(RCB[resourceNum]->maxAvaliability < amount){ // ERROR CASE
            std::cout << "*--Requesting too many resource, max: " << RCB[resourceNum]->maxAvaliability << 
                ", but got " << amount << " instead" << std::endl;
            put(-1);
            return;
        }
        if(holding == NULL){
            std::string msg;
            RCB[resourceNum]->addToWaitlist(currentPCB, amount);
            msg = removeFromReadyList(currentPCB); // call reschedule inside this funciton;
            if(msg.size() != 0){
                std::cout << msg << std::endl;
            }
        }
        else{
            if(holding->amount + amount > RCB[resourceNum]->maxAvaliability){
                std::cout << "*--Requesting too many resource, max: " << RCB[resourceNum]->maxAvaliability << 
                    ", but holding: " << holding->amount << " while requesting: " << amount << std::endl;
                put(-1);
                return;
            }
            else{
                std::string msg;
                RCB[resourceNum]->addToWaitlist(currentPCB, amount);
                msg = removeFromReadyList(currentPCB); // call reschedule inside this funciton; 
                if(msg.size() != 0){
                    std::cout << msg << std::endl;
                }        
            }
        }
    }
    put(currentPCB);
}

void release(int resourceNum, int amount){
    if(resourceNum < 0 || resourceNum >= RCBTOTAL){
        std::cout << "*--Invalid resource number, expect [0," << RCBTOTAL-1 << 
            "] but got " << resourceNum << " instead" << std::endl;
        put(-1);
        return;
    }
    
    // Find resource in this PCB
    struct LLResource* holding = PCB[currentPCB]->resources;
    while(holding != NULL){
        if(holding->num == resourceNum){
            break;
        }
        holding = holding->next;
    }

    // Checking cases
    if(holding == NULL){
        std::cout << "*--Process " << currentPCB << " does not hold resource " << resourceNum << std::endl;
        put(-1);
        return;
    }
    else if(holding->amount < amount){
        std::cout << "*--Process " << currentPCB << " only holds " << holding->amount <<
            " of current resource, but trying to free " << amount << std::endl;
        put(-1);
        return;
    }
    else{ 
        holding->amount -= amount;
        if(holding->amount == 0){
            PCB[currentPCB]->deleteResource(resourceNum);
        }
        RCB[resourceNum]->restore(amount);
        struct LLResource* waitlistItem = RCB[resourceNum]->waitlist;

        bool reschedule = false;
        while(waitlistItem != NULL && RCB[resourceNum]->avaliable > 0){
            std::cout << "Request: " << waitlistItem->amount << ", Avaliable: " 
                    << RCB[resourceNum]->avaliable << std::endl;
            if(waitlistItem->amount <= RCB[resourceNum]->avaliable){
                std::cout << waitlistItem->num << ", " << PCB[waitlistItem->num]->priority << std::endl;
                addToReadyList(waitlistItem->num, PCB[waitlistItem->num]->priority);
                PCB[waitlistItem->num]->appendResource(resourceNum, waitlistItem->amount);
                RCB[resourceNum]->removeFromWaitlist(waitlistItem);
                reschedule = true;
            }
            else{
                break; // break to prevent lock
            }
            std::cout << "Request222: " << waitlistItem->amount << ", Avaliable: " 
                    << RCB[resourceNum]->avaliable <<",\t WaitNext is NULL: " <<
                    (waitlistItem->next == NULL) << std::endl;

            waitlistItem = RCB[resourceNum]->waitlist;
        }
        if (reschedule) scheduler();
    }
    put(currentPCB);
}


void timeout(){
    struct LL* rl;
    for(int i = RLTOTAL-1; i >= 0; i--){
        rl = RL[i];
        if(rl != NULL){
            if(rl->value != currentPCB){
                std::cout << "*--Current running process " << currentPCB << 
                    " is not at the top of the Ready List. Top of Ready List is " <<
                    rl->value << "." << std::endl;
                put(-1);
                return;
            }
            if(rl->next == NULL){ // The only process in this priority;
                std::cout << "*Process " << currentPCB << " is running, No scheduler called" << std::endl;
                put(currentPCB);
                return;
            }
            RL[i] = rl->next;
            rl->next = NULL;
            RL[i]->add(rl->value);
            scheduler();
            delete rl;
            put(currentPCB);
            return;
        }
    }
}

void scheduler(){
    struct LL* rl;
    for(int i = RLTOTAL-1; i >= 0; i--){
        rl = RL[i];
        if(rl != NULL){
            if(rl->value != currentPCB){
                currentPCB = rl->value;
                std::cout << "*Process " << currentPCB << " is running" << std::endl;
            }
            else{
                std::cout << "*Same ReadyList value, currently running: " << currentPCB << std::endl;
            }
            return;
        }
    }
}

void addToReadyList(int pcbNum, int priorityLevel)
{
    std::cout <<"====" << pcbNum <<","<< priorityLevel<<std::endl;
    if(pcbNum == 0){
        return;
    }
    if(RL[priorityLevel] == NULL){
        std::cout << "***FIRST" << std::endl;
        RL[priorityLevel] = new LL(pcbNum);
    }
    else{
        std::cout << "***SECOND"<<pcbNum << std::endl;
        RL[priorityLevel]->add(pcbNum);
    }
}

std::string removeFromReadyList(int pcbNum)
{
    struct LL *currLL, *prevLL;
    for(int i = 0; i < RLTOTAL; i++){
        currLL = prevLL = RL[i];
        if(currLL == NULL) continue;
        if(currLL->value == pcbNum){
            RL[i] = currLL->next;
            currLL->next = NULL;
            delete currLL;
            scheduler();
            return "";
        }
        currLL = currLL->next;
        while(currLL != NULL){
            if(currLL->value == pcbNum){
                prevLL->next = currLL->next;
                currLL->next = NULL;
                delete currLL;
                scheduler();
                return "";
            }
            prevLL = currLL;
            currLL = currLL->next;
        }
    }
    return "*--Can not find Process #" + std::to_string(pcbNum);
}

void printReadyList()
{
    struct LL* rl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++" << std::endl;
    for(int i = RLTOTAL-1; i >= 0; i--){
        std::cout << "===== ReadyList " << i <<" ====="<< std::endl;
        rl = RL[i];
        while(rl!= NULL){
            std::cout << rl->value << ", ";
            rl = rl->next;
        }
        std::cout << std::endl;
    }
    std::cout << "++++++++++++++++++++++++++++++++++++++++" << std::endl;
}

void printResourceWaitList(){
    struct LLResource* wl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++" << std::endl;
    for(int i = RCBTOTAL-1; i >= 0; i--){
        std::cout << "===== Resource Number: " << i << ", Holding: " << RCB[i]->avaliable <<" ====="<< std::endl;
        wl = RCB[i]->waitlist;
        while(wl!= NULL){
            std::cout << "(" << wl->num << ", " << wl->amount << ")" << std::endl;
            wl = wl->next;
        }
        std::cout << std::endl;
    }
    std::cout << "++++++++++++++++++++++++++++++++++++++++" << std::endl;
}
