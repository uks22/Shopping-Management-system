# Walmart Store System Design

## Overview

The Walmart Store system is designed to efficiently manage store inventory and also Identify the pattern of Customer for buying Items. I have coded the given problem statement separately for both Data Structures i.e. B+ trees and Linked List. For each aisle, there is a tree. The nodes in the tree are the items in that aisle with information like the item id, item_name, quantity, expiry_date, threshold quantity.

## Files and Structures

- **shopping(B+trees).c**: Implmented and used B+ trees for managing sections and items within each aisle.
- **dspd3.c**: Implemented and used linked list structures for managing bills and items within each aisle.

## Implemented Functions

1. **Add / Update Item**
   - Adds or updates an item in a specific aisle.
   
2. **Delete Item**
   - Deletes an item from a specific aisle.

3. **Add Aisle / Update Aisle**
   - Adds a new aisle or updates an existing aisle.

4. **Delete Aisle**
   - Deletes an aisle from collection of existing aisles in Store.

5. **Check Availability of Item**
   - Checks if a particular item is available in sufficient quantity and within the expiry date specified.

6. **Threshold Quantity Notification**
   - Generates a message when any particular item's quantity goes beyond its defined threshold, indicating the need for reordering.

7. **Expiry Notification**
   - Generates messages for items such as ready-to-eat foods nearing expiry (within one week) or dairy products likely to expire soon (within one day).

8. **Merge Aisles**
   - Merges two aisles into a single aisle to optimize space or accommodate new sections like over-the-counter medicines.

9. **Related Items Recommendation**
   - Takes an item_id as input and provides a list of items that are frequently bought together with the given item_id, using historical transaction data.

## Example Aisles in Walmart

- Aisle 1: Dairy Products
- Aisle 2: Pulses and Grains
- Aisle 3: Bath and Cleaning Products
- Aisle 4: Ready-to-Eat Food Items
- Aisle 5: Vegetables

## Data Structures Used

- **B+ Trees** : Since in Walmart stores there are different section and each section has various items, I have used nested B+ Trees i.e. each section in store is managed using B+ Tree and each node of B+ Tree itself has many items which itself has another B+ Trees.
- **Linked List** : I have also separately coded it using Linked List as Data Structure. It utilizes array of Linked List i.e. separate Linked List for each Aisle in form of array.

## Integration and Usage

- Users interact with the system through various functions provided, such as adding/updating items, deleting items, checking availability, receiving notifications, and merging aisles.
- Data structures ensure efficient storage and retrieval of items and sections, maintaining order and facilitating quick access to required information.

---

