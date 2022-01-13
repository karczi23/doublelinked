#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <cstdio>


struct Node {
  Node *p = nullptr;

  std::string data;

  Node(std::string _data) { data = _data; }
};

Node *XOR(Node *a, Node *b) {
  return (Node *)((uintptr_t)(a) ^ (uintptr_t)(b));
}

struct List {
  Node *head = nullptr;
  Node *tail = nullptr;

  Node *selected = nullptr;

  Node *leading = nullptr;
  Node *trailing = nullptr;

  void insert(Node **list, std::string data) {
    Node *newnode = new Node(data);

    if (*list != nullptr) {
      (*list)->p = XOR((*list)->p, newnode);
      newnode->p = *list;
    } else {
      selected = head = tail = newnode;
    }

    *list = newnode;

    if (&head == list && head->p == selected) {
      leading = head;
    }

    if (&tail == list && tail->p == selected) {
      trailing = tail;
    }
  }

  void insert(Node **previous, Node **selected, Node **list, std::string data) {
      Node *newnode = new Node(data);
        if (*previous) {
            (*previous)->p = XOR(XOR((*previous)->p, *selected), newnode);
            (*selected)->p = XOR(XOR((*selected)->p, *previous), newnode);
            newnode->p = XOR(*previous, *selected);
            *previous = newnode;
        } else {
            insert(list, data);
        }
  }

  void remove(Node **list) {
    if (!*list)  // lista jest już pusta
      return;

    Node *removed = *list;  // node do usunięcia

    if (removed->p) {  // istnieje kolejny node
      *list = removed->p;
      (*list)->p = XOR((*list)->p, removed);
    } else {  // to był ostani node, lista jest teraz pusta
      selected = head = tail = nullptr;
    }

    if (removed == trailing) {
      trailing = nullptr;
    }
    if (removed == leading) {
      leading = nullptr;
    }
    if (removed == selected) {
        selected = tail;
        leading = nullptr;
        trailing = selected->p;
    }

    if (removed == selected && removed->p) {
      selected = removed->p;

      if (selected == head) {
        trailing = selected->p;
      }
      if (selected == tail) {
        leading = selected->p;
      }
    }    

    delete removed;
  }
  
void del_selected(Node **next, Node **previous) {
        Node *removed = selected;
        if (!(*previous) && !(*next)) {
            delete removed;
            head = tail = trailing = leading = selected = nullptr;
            return;
        }        
        
        if (!head) {
            return;
        }
        
        if (!(*previous)) {
            head = selected->p;
            head->p = XOR((*next)->p, selected);
            selected = tail;
            *next = nullptr;
            *previous = selected->p;
            
            delete removed;       
            return;
        }
        
        if (*next == nullptr && XOR((*previous)->p, selected) == nullptr) {
            head = tail = selected = *previous;
            selected->p = nullptr;
            *next = *previous = nullptr;
            delete removed;
            return;
        }
        
        if (*next == nullptr) {
            (*previous)->p = XOR(XOR((*previous)->p, selected), *next);
            selected = *previous;
            tail = selected;
            *previous = XOR(selected->p, *next);
            delete removed;
            return;
        }
        (*previous)->p = XOR(XOR((*previous)->p, selected), *next);
        (*next)->p = XOR(XOR((*next)->p, selected), *previous);
        selected = *previous;
        *previous = XOR(selected->p, *next);

        delete removed;
        return;
    }

    void del_value(Node **next, Node **previous, std::string data) {
        if (selected->data == data) {
            del_selected(next, previous);
        }
        if (selected == nullptr)
            return;
        Node **prevtmp = previous;
        Node *currtmp = selected;
        Node **nexttmp = next;
        while (true) {

            advance(next, previous, head, false);
            if (selected->data == data) {
                del_selected(next, previous);
            }
            if (selected->data == "") {
                next = nullptr;
                selected = nullptr;
                previous = nullptr;
                break;
            }
            if (selected == currtmp) {
                next = nexttmp;
                previous = prevtmp;
                selected = currtmp;
                break;
            }
        }
    }

  void print(Node *list) {
    Node *last = nullptr, *current = list, *next = nullptr;
    
    if (!current) {
        std::cout << "NULL";
    }

    while (current) {
      std::cout << current->data << " ";
      next = XOR(last, current->p);
      last = current;
      current = next;
    }
    std::cout << std::endl;
  }

  void print(Node* list, bool isSelected) {
      if (selected && isSelected)
        std::cout << selected->data << std::endl;
      else
        std::cout << "NULL" << std::endl;
  }

  void advance(Node **next, Node **previous, Node *list, bool print) {
    if (selected == nullptr) {
        if (print)
            std::cout << "NULL" << std::endl;
        return;
    }
    if (*next == nullptr) {
        selected = list;
        *next = selected->p;
        *previous = nullptr;
            if (print)
                std::cout << selected->data << std::endl;
            return;
    }
    Node *advanced = *next;
    *next = XOR((*next)->p, selected);
    *previous = selected;
    selected = advanced;
    if (print)
        std::cout << selected->data << std::endl;
  }

};

void ADD_BEG(List *l, std::string data) { l->insert(&l->head, data); }
void ADD_END(List *l, std::string data) { l->insert(&l->tail, data); }
void ADD_ACT(List *l, std::string data) { l->insert(&l->leading, &l->selected, &l->head, data); }
void ACTUAL(List *l) { l->print(l->head, true); }
void PREV(List *l) { l->advance(&l->leading, &l->trailing, l->tail, true); }
void NEXT(List *l) { l->advance(&l->trailing, &l->leading, l->head, true); }
void DEL_BEG(List *l) { l->remove(&l->head); }
void DEL_ACT(List *l) { l->del_selected(&l->trailing, &l->leading); }
void DEL_END(List *l) { l->remove(&l->tail); }
void DEL_VAL(List *l, std::string data) { l->del_value(&l->trailing, &l->leading, data); }
void PRINT_FORWARD(List* l) { l->print(l->head); }
void PRINT_BACKWARD(List* l) { l->print(l->tail); }

int main() {
  List list = List();

    std::string input = "a";

    while (input != "") {
        input = "";
        
        std::string first = "";
        std::string parameter = "";
        
        getline(std::cin, input);
        
        std::size_t found_f = input.find_first_of(" ");
        std::size_t found = input.find_last_of(" ");
        first = input.substr(0, found_f);
        parameter = input.substr(found+1);

        if (first == "ADD_BEG" || first == "ADD_END" || first == "ADD_ACT" || first == "DEL_VAL") {
            if (first == "ADD_BEG")
                ADD_BEG(&list, parameter);
            if (first == "ADD_END")
                ADD_END(&list, parameter);
            if (first == "ADD_ACT")
                ADD_ACT(&list, parameter);
            if (first == "DEL_VAL")
                DEL_VAL(&list, parameter);
        } else {
            if (input == "ACTUAL")
                ACTUAL(&list);
            if (input == "NEXT")
                NEXT(&list);
            if (input == "PREV")
                PREV(&list);
            if (input == "DEL_BEG")
                DEL_BEG(&list);
            if (input == "DEL_END")
                DEL_END(&list);
            if (input == "DEL_ACT")
                DEL_ACT(&list);
            if (input == "PRINT_FORWARD")
                PRINT_FORWARD(&list);
            if (input == "PRINT_BACKWARD")
                PRINT_BACKWARD(&list);
        }
    }

}