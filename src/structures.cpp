#include <iostream>

#include "structures.hpp"

LL::LL(int value)
{
    this->value = value;
    this->next = NULL;
}

LL::~LL()
{
    if(this->next != NULL) delete this->next;
}

void LL::add(int nextVal)
{
    struct LL* thisNode = this;
    while(thisNode->next != NULL){
        thisNode = thisNode->next;
    }
    thisNode->next = new LL(nextVal);
}

LLResource::LLResource(int num, int amount)
{
    this->num = num;
    this->amount = amount;
    this->next = NULL;
}

LLResource::~LLResource()
{
    if(this->next != NULL) delete this->next;
}

void LLResource::add(LLResource *next)
{
    struct LLResource* thisNode = this;
    while(thisNode->next != NULL){
        thisNode = thisNode->next;
    }
    thisNode->next = next;
}

Resource::Resource(int maxAvaliability)
{
    this->avaliable = maxAvaliability;
    this->maxAvaliability = maxAvaliability;
    this->waitlist = NULL;
}

Resource::~Resource()
{
    if(this->waitlist != NULL) delete this->waitlist;
}

void Resource::request(int num)
{
    if(num < this->avaliable) std::cout << "*--Requesting too many resource" << std::endl;
    else    this->avaliable -= num;
}

void Resource::restore(int num)
{
    if(this->avaliable + num > this->maxAvaliability)
        std::cout << "*--Returning too many resource" << std::endl;
    else
        this->avaliable += num;
}

void Resource::addToWaitlist(int pcbNum, int amountRequested)
{
    if(this->waitlist == NULL){
        this->waitlist = new LLResource(pcbNum, amountRequested);
    }
    else{
        this->waitlist->add(new LLResource(pcbNum, amountRequested));
    }
}

void Resource::removeFromWaitlist(LLResource *resource)
{
    this->avaliable -= resource->amount;
    if(this->waitlist == resource){
        this->waitlist = resource->next;
    }
    else{
        struct LLResource* prev = this->waitlist;
        while(prev->next != resource){
            prev = prev->next;
        }
        prev->next = resource->next;
    }
    resource->next = NULL;
    delete resource;
}

bool Resource::removeFromWaitlist(int pcbNum)
{
    struct LLResource *curr, *prev;
    curr = prev = this->waitlist;
    if(curr == NULL) return false;
    if(curr->num == pcbNum){
        this->waitlist = curr->next;
        curr->next = NULL;
        delete curr;
        return true;
    }
    else{
        curr = curr->next;
        while(curr != NULL){
            if(curr->num == pcbNum){
                prev->next = curr->next;
                curr->next = NULL;
                delete curr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    return false;
}

ProcessControlBlock::ProcessControlBlock(int parent, int priority)
{
    this->parent = parent;
    this->priority = priority;
    childs = NULL;
    resources = NULL;
}

ProcessControlBlock::~ProcessControlBlock()
{
    if(this->childs != NULL) delete this->childs;
    if(this->resources != NULL) delete this->resources;
}

ProcessControlBlock *ProcessControlBlock::createChild(int childNum, int priority)
{
    ProcessControlBlock* child = new ProcessControlBlock(childNum, priority);
    if(this->childs == NULL){
        this->childs = new LL(childNum);
    }
    else{
        this->childs->add(childNum);
    }
    return child;
}

void ProcessControlBlock::appendResource(int resourceNum, int amount)
{
    if(resources == NULL){
        resources = new LLResource(resourceNum, amount);
    }
    else{
        resources->add(new LLResource(resourceNum, amount));
    }
}

void ProcessControlBlock::deleteResource(int resourceNum)
{
    struct LLResource* thisResource = this->resources, *prevResource;
    if(thisResource->num == resourceNum){
        this->resources = thisResource->next;
        thisResource->next = NULL;
        delete thisResource;
        return;
    }
    prevResource = thisResource;
    thisResource = thisResource->next;
    while(thisResource != NULL){
        if(thisResource->num == resourceNum){
            prevResource->next = thisResource->next;
            thisResource->next = NULL;
            delete thisResource;
            return;
        }
    }
    std::cout << "*--Resource " << resourceNum << " Not found in this process" << std::endl;
}

// void ProcessControlBlock::requestResources(int resourceNum, int amount, int pcbNum)
// {
//     struct Resource *resource = getRCB(resourceNum); // TO DO
//     if(resource->maxAvaliability < amount){ // ERROR CASE
//         std::cout << "*Requesting too many resources." << std::endl;
//         std::cout << "*\t Requesting: " << amount << " of resource " << resourceNum <<
//             ", but there are only " << resource->maxAvaliability << 
//             " of this resource avaliable" << std::endl;
//     }
//     else{
//         struct LLResource* holding = this->resources;
//         while(holding != NULL){
//             if(holding->num == resourceNum){
//                 break;
//             }
//             holding = holding->next;
//         }
//         if(holding != NULL){
//             if(holding->amount + amount > resource->maxAvaliability){ // ERROR CASE
//                 std::cout << "*Requesting too many resources" << std::endl;
//                 std::cout << "*\tHolding: " << holding->amount <<
//                     ", Requesting: " << amount <<
//                     ", Total avaliable resource: " << resource->maxAvaliability<<std::endl;
//             }
//             else{
//                 std::cout << "*Requesting " << amount << " resources" << std::endl;
//                 if(amount <= resource->avaliable){
//                     resource->request(amount);
//                     holding->amount += amount;
//                     std::cout << "*Received " << amount << " resources, currently holding "<<
//                         holding->amount + amount << " of resource #" << resourceNum << std::endl;
//                 }
//                 else{
//                     resource->addToWaitlist(pcbNum, amount);
//                     std::cout << "*Resource not avaliable, puting current process into waitlist" << std::endl;
//                 }
//             }
//         }
//         else{
//             if(amount <= resource->avaliable){
//                 if(this->resources == NULL){
//                     this->resources = new LLResource(resourceNum, amount);
//                 }
//                 std::cout << "*Received " << amount << " resources, currently holding "<<
//                     amount << " of resource #" << resourceNum << std::endl;
//             }
//             else{
//                 resource->addToWaitlist(pcbNum, amount);
//                 std::cout << "*Resource not avaliable, puting current process into waitlist" << std::endl;
//             }
//         }
//     }
// }
