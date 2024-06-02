Using B+ trees(in shopping(B+trees).c file) and a linked list data structure(in dspd3.c file),I design and develop a system for Walmart Store.Using B+ tree data structure,I design and develop a system for Walmart Store. I implement nested B+ trees where a tree of different section in Walmart is created consisting of nodes of section which itself is another B+ tree consisting of nodes of different items in that section. In Walmart Store, there are different aisles. In one aisle there are dairy products. In the other there are pulses and 
grains. In the third there are bath and cleaning products. In fourth aisle they have ready to eat food items. In 5th aisle there are vegetables. I Maintained an array of linked lists for aisles. For each aisle, there is a linked list. The nodes in the linked list are the items in that aisle like the item id, item_name, quantity, expiry_date, threshold quantity. Now there is another linked list of bills. When a user comes to the Walmart store, he/she is billed for whatever items he/she buys. The corresponding quantity of each item is to be decremented in the main array of linked list of items. The array of aisles is sorted according to the aisle_number. Items are sorted by item_id in each aisle.
Implemented functions :
• Add / update item – Adds or updates an item in the linked list. 
• Delete an item – Delete the item 
• Add aisle/update aisle – Adds or updates an aisle
• Delete aisle – Deletes aisle 
• Check availability of particular item for particular quantity and within the expiry date. 
• Inputs – Item_id, quantity required for item_id and the expiry date
• Function to generate a message when any particular item goes beyond its defined threshold 
quantity, so that it can be planned to order it soon.
• Function to generate a message on a certain ready-to-eat item if its expiry is within one 
week.
• Function to generate a message when certain dairy product is likely to expire within one day. 
• Merge two aisles in a single aisle. For example, Wallmart wishes to add over the counter 
medicine section to the store. So, they have less space and want to merge their two aisles, 
namely, dairy and vegetables.
• Write a function that takes item_id as input and provides a list of items which are more 
often bought with the given item_id In Walmart Store, there are different aisles. In one aisle there are dairy products. In the other there are pulses and 
grains. In the third there are bath and cleaning products. In fourth aisle they have ready to eat food items. In 5th aisle there are vegetables. I Maintained an array of linked lists for aisles. For each aisle, there is a linked list. The nodes in the linked list are the items in that aisle like the item id, item_name, quantity, expiry_date, threshold quantity. Now there is another linked list of bills. When a user comes to the Walmart store, he/she is billed for whatever items he/she buys. The corresponding quantity of each item is to be decremented in the main array of linked list of items. The array of aisles is sorted according to the aisle_number. Items are sorted by item_id in each aisle.
Implemented functions :
• Add / update item – Adds or updates an item in the linked list. 
• Delete an item – Delete the item 
• Add aisle/update aisle – Adds or updates an aisle
• Delete aisle – Deletes aisle 
• Check availability of particular item for particular quantity and within the expiry date. 
• Inputs – Item_id, quantity required for item_id and the expiry date
• Function to generate a message when any particular item goes beyond its defined threshold 
quantity, so that it can be planned to order it soon.
• Function to generate a message on a certain ready-to-eat item if its expiry is within one 
week.
• Function to generate a message when certain dairy product is likely to expire within one day. 
• Merge two aisles in a single aisle. For example, Wallmart wishes to add over the counter 
medicine section to the store. So, they have less space and want to merge their two aisles, 
namely, dairy and vegetables.
• Write a function that takes item_id as input and provides a list of items which are more 
often bought with the given item_id
