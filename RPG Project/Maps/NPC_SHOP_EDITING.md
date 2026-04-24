DISCLAIMER: i did have AI summarize this because I really didn't want to do it myself, but it is correct and should give accurate info





# NPC and Shop Editing Guide

This guide explains how to edit NPC dialogue and shop inventory directly in a map JSON file.

## 1) Open the correct map file

Edit the map file you actually load in-game (usually in this folder):
- RPG Project/Maps

If you have duplicate map files in other folders, make sure you edit the same one the game is loading.

## 2) Find the entity you want to edit

Inside the JSON, find the `entities` array.

Example:

```json
"entities": [
  {"id": 20, "chunkX": 3, "chunkY": 0, "tileX": 13, "tileY": 2, "type": "shop"},
  {"id": 29, "chunkX": 3, "chunkY": 0, "tileX": 12, "tileY": 9, "type": "npc"}
]
```

Take note of the `id`.

## 3) Add or edit matching metadata

Inside the same file, use the `entityMetadata` array.

Each metadata object must match an entity `id` using `entityId`.

Example:

```json
"entityMetadata": [
  {
    "entityId": 29,
    "data": {
      "npcName": "Guard",
      "npcDescription": "A city guard.",
      "dialogue": "No one gets in without a pass.",
      "xpReward": 0,
      "shopName": "",
      "shopNpcName": "",
      "shopInventory": ""
    }
  },
  {
    "entityId": 20,
    "data": {
      "npcName": "",
      "npcDescription": "",
      "dialogue": "",
      "xpReward": 0,
      "shopName": "Starter Shop",
      "shopNpcName": "Mara",
      "shopInventory": "1:10,2:3"
    }
  }
]
```

## 4) Field meanings

- `type`:
  - `"npc"` for NPCs
  - `"shop"` for shops
- `dialogue`: text shown when interacting with an NPC
- `shopName`: shop display name in popup
- `shopInventory`: comma-separated list in `itemId:quantity` format
  - Example: `"1:10,2:3"` means item 1 x10 and item 2 x3

## 5) Save and test

1. Save the JSON file.
2. Load that map in game.
3. Interact with the NPC/shop to verify changes.

## 6) Common mistakes

- `entityId` does not match any entity `id`
- Broken JSON (missing comma, quote, or brace)
- Using wrong inventory format (must be `id:qty,id:qty`)
- Editing a duplicate map file that is not the one currently loaded

## 7) Reset snapshot note

Your map now creates a reset snapshot file the first time it is loaded:
- `<mapname>_reset.json`

If you want a NEW reset baseline after major edits, delete the old `_reset.json` file and load the map again so a fresh snapshot is created.
