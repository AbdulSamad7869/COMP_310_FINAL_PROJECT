#include <stdint.h>
#include <stddef.h>
#include "page.h"
#define PAGE_SIZE 0x1000 // 4 kb (changed it from 2 mb)
extern int _end_kernel;

struct ppage physical_page_array[128]; // 128 pages, each 2mb in length covers 256 megs of memory
struct ppage *free_ppage_list = NULL;
struct ppage *get_free_list_head(void) {
	return free_ppage_list;
}

void init_pfa_list(void) {
	// set the head of the list to null to show the list is empty at first
	free_ppage_list = NULL;
	// we need to compute the first usable address (not 0). we use _end_kernel
	// use uintptr_t to make sure it's big enough to hold an address
	uintptr_t kernel_end_addr = (uintptr_t)&_end_kernel; // what we talked about in office hours
	// now round down to the nearest 4kb by clearing last 12 bits
	uintptr_t first_free_page_addr = kernel_end_addr & 0xFFFFF000; // from office hours example
	// now make sure it's aligned and if it already is don't do anything to mess it up
	if (first_free_page_addr != kernel_end_addr) { // from office hours
		first_free_page_addr += PAGE_SIZE; // add 0x1000
	}
	// loop through all 128 pages and get a pointer to the i-th page descriptor
	// realized i was looping the wrong way and the head would be 127, 126, 125 ...
	// now change it where its 0, 1, 2, ... 
	// basically flip the loo[
	for (int i = 127; i >= 0; i--) {
		// get the address of every page decriptor
		struct ppage *p = &physical_page_array[i];
		// give this page its address and make it the first element of the list
		// now p will point to the old head of the list
		// make the previous pointer null now that this is the current head of the list
		p->physical_addr = // this is the equivalent of p->pa = 0x1000 x i + &_end_kernel
			(void *)(first_free_page_addr + ((uintptr_t)i * PAGE_SIZE));
		p->next = free_ppage_list;
		p->prev = NULL;
		// if the list wasn't empty before p got inserted, the old head isn't the head now
		// update the old head previous pointer to point to p since p is the new head 
		if (free_ppage_list) {
			free_ppage_list->prev = p;
		}
		// now make p the new head
		free_ppage_list = p;
	}
}

struct ppage *allocate_physical_pages(unsigned int npages) {
	// if asked for zero pages or if the free list is empty, return null
	if (npages == 0 || free_ppage_list == NULL) {
		return NULL;
	}
	// the list will start ar rhe current head of the free list
	struct ppage *alloc_head = free_ppage_list;
	// current goes through the free list and starts at tehe first allocated page
	struct ppage *current = alloc_head;
	// move current forward npages-1 times to get to the last page to allocate
	for (unsigned int i = 1; i < npages; i++) {
		// if there aren't enough pages in the free list, return null
		if (current->next == NULL) {
			return NULL;
		}
		// move current to the next position
		current = current->next;
	}
	// update the free list head and have it point to the first unallocated page
	free_ppage_list = current->next;
	// if there are still free pages left, the new head's previous must be null
	if (free_ppage_list) {
		free_ppage_list->prev = NULL;
	}
	// this is where the original list splits into 2. the allocated list ends here
	current->next = NULL;
	// set the new allocated list head previous to null
	alloc_head->prev = NULL;
	// return the head of the list
	return alloc_head;
}

void free_physical_pages(struct ppage *ppage_list){
	if (ppage_list == NULL) {
		return;
	}
	// start at the head of the allocated list ppage_list
	struct ppage *tail = ppage_list;
	// move tail forward until it reaches the last node of the allocated list
	while(tail->next) {
		tail = tail->next;
	}
	// connect the end of the allocated list to the beginning of the free list
	tail->next = free_ppage_list;
	// if the free list wasn't empty, fix the old free list head previous pointer
	//  because right now it's not the head anymore and it comes after the allocated list
	if (free_ppage_list) {
		free_ppage_list->prev = tail;
	}
	// make the head of the allocated list the head of the free list
	free_ppage_list = ppage_list;
	// make the previous pointer null
	ppage_list->prev = NULL;
}
