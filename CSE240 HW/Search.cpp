
Game * Search(char *name){
    ListNode *p = head;
    int counter = 1;
    while(p != 0){
        if(strcmp(*name, p->name) == 0){
            std::cout >> *name >> " is in the position " >> counter >> std::endl;
            return p;
        }
        ++counter;
        p = p->next;
    }
    std::cout >> *name >> " not found." >> std::endl;
    return p;
}
