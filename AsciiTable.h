/*

Usage Example
-------------

    AsciiTable table = beginTable();

    addColumn( &table, "string Value" );
    addColumn( &table, "double Value" );
    addColumn( &table, "int Value" );

    addRow( &table );
    addValue( &table, "String" );
    addValue( &table, 1.1 );
    addValue( &table, 2 );

    addRow( &table );
    addValue( &table, "String2" );
    addValue( &table, 2.2 );
    addValue( &table, 9 );

    endTable( &table, std::cout );

*/
#pragma once

#include <iomanip>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

/*
    TODO(joe):
        - Consolidate the column separator 
        - Handle when the row value's are wider than the column headers.
        - Handle when there are more values in a row than columns

        - This leans heavily on the STL libraries which might not be helpful on
          projects that aren't compiled in Visual Studio 2012 on Windows.
*/

struct AsciiTable
{
    std::vector<std::string> arrColumns;
    std::vector<int> arrColumnWidths;
    // TODO(joe): This needs to be a pointer to avoid an issue with stringstream and vectors. (copyable?)
    // Look into using something other that stringstream.
    std::vector<std::shared_ptr<std::stringstream>> arrRows;

    size_t nCurrentRow;
    size_t nCurrentCol;
};

AsciiTable beginTable()
{
    AsciiTable result = {};

    return result;
}

void addColumn( AsciiTable* pTable, const std::string& strColumnName )
{
    pTable->arrColumns.push_back( strColumnName );
    pTable->arrColumnWidths.push_back( (int)strColumnName.size() + 2 ); // +2 is for the column padding
}

void addRow( AsciiTable* pTable )
{
    pTable->arrRows.push_back( std::shared_ptr<std::stringstream>( new std::stringstream() ) );

    pTable->nCurrentRow = pTable->arrRows.size() - 1;
    pTable->nCurrentCol = 0;
}

void addColumnSeparator( AsciiTable* pTable, bool first )
{
    if (first)
    {
        *pTable->arrRows[pTable->nCurrentRow] << "| ";
    }
    else
    {
        *pTable->arrRows[pTable->nCurrentRow] << " | ";
    }
}

void addValue( AsciiTable* pTable, int nValue )
{
    addColumnSeparator( pTable, pTable->nCurrentCol == 0 );

    int columnWidth = pTable->arrColumnWidths[pTable->nCurrentCol++];
    *pTable->arrRows[pTable->nCurrentRow] << std::setw(columnWidth-2) << nValue;
}

void addValue( AsciiTable* pTable, double value )
{
    addColumnSeparator( pTable, pTable->nCurrentCol == 0 );

    int columnWidth = pTable->arrColumnWidths[pTable->nCurrentCol++];
    *pTable->arrRows[pTable->nCurrentRow] << std::setw(columnWidth-2) << value;
}

void addValue( AsciiTable* pTable, const std::string& strValue )
{
    addColumnSeparator( pTable, pTable->nCurrentCol == 0 );

    int columnWidth = pTable->arrColumnWidths[pTable->nCurrentCol++];
    *pTable->arrRows[pTable->nCurrentRow] << std::setw(columnWidth-2) << strValue;
}

std::string getRowSeparator( AsciiTable* pTable )
{
    std::stringstream ss;
    ss << "+";

    for ( size_t columnIndex = 0; columnIndex < pTable->arrColumns.size(); ++columnIndex )
    {
        for ( int dashCount = 0; dashCount < pTable->arrColumnWidths[columnIndex]; ++dashCount )
        {
            ss << "-";
        }
        ss << "+";
    }

    return ss.str();
}

void endTable( AsciiTable* pTable, std::ostream& output )
{
    std::string rowSeparator = getRowSeparator( pTable );

    // Print the table header
    output << rowSeparator << std::endl;
    output << "| ";
    for ( size_t columnIndex = 0; columnIndex < pTable->arrColumns.size(); ++columnIndex )
    {
        output << pTable->arrColumns[columnIndex];

        if ( columnIndex < pTable->arrColumns.size()-1 )
        {
            output << " | ";
        }
    }
    output << " |" << std::endl;
    output << rowSeparator << std::endl;

    // for each row print each column value
    for ( size_t rowIndex = 0; rowIndex < pTable->arrRows.size(); ++rowIndex )
    {
        output << pTable->arrRows[rowIndex]->str() << " |" << std::endl;
        output << rowSeparator << std::endl;
    }
}
