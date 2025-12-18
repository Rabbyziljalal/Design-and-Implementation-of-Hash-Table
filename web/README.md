# Phone Directory Web Application

## Quick Start

Your web-based phone directory is ready! 

### Option 1: Direct File Open (Simplest)
Simply open the file in your browser:
```
c:\Users\rabby\Desktop\Design and Implementation of Hash Table\web\index.html
```

Double-click `index.html` or drag it into your browser. All features work without a server!

### Option 2: Local Server (Recommended for development)

If you have Node.js installed:
```powershell
cd "c:\Users\rabby\Desktop\Design and Implementation of Hash Table\web"
npx http-server -p 8000 -o
```

If you have Python installed:
```powershell
cd "c:\Users\rabby\Desktop\Design and Implementation of Hash Table\web"
python -m http.server 8000
# Then open: http://localhost:8000
```

## Features

✅ **Insert Records** - Add new phone directory entries
✅ **Search Records** - Search by username or phone number  
✅ **Delete Records** - Remove entries from the directory
✅ **Dual Hash Tables** - Separate tables for username and phone indexing
✅ **Hash Algorithm** - Modulo-division hash: `hash(key) = (Σ ASCII) % 30`
✅ **Collision Resolution** - Linear probing: `h(k,i) = (h(k) + i) mod m`
✅ **Data Persistence** - Auto-save to browser localStorage
✅ **Sample Data** - Load 30 test records instantly
✅ **Export/Import** - Download and upload JSON data files
✅ **Real-time Stats** - View record count and load factor
✅ **Responsive Design** - Works on desktop, tablet, and mobile

## Technical Details

- **Table Size**: 30 (matches record count)
- **Hash Function**: Sum of ASCII values modulo table size
- **Collision Handling**: Linear probing with lazy deletion
- **Storage**: Browser localStorage for persistence
- **No Backend Required**: Pure client-side JavaScript

## Usage

1. **Insert a Record**: Click "Insert" tab, fill in username, phone, and address
2. **Search**: Click "Search" tab, choose search type, enter key
3. **Delete**: Click "Delete" tab, choose delete type, enter key
4. **View Tables**: Click buttons to display username/phone hash tables
5. **Load Sample Data**: Click "Data" tab → "Load Sample Data"

## Student Information
- **Student ID**: F23040111
- **Implementation**: Hash Table with Modulo-Division + Linear Probing
- **Language**: JavaScript (ES6+)
- **Framework**: Vanilla JS (no dependencies)
