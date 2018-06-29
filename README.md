# CYCLUS2XLS

This is a simple tool to feel an `XLS` spreadsheet with data from a `Cyclus` output.

One need to provide an input file (containing the list of the data that will be
written in the `XLS` spreadsheet and where) and a `Cyclus` output file.

The script input file needs to be format as follow:
 - First a part with generic informations:
    
    - XLS template file, using `%template:`
    - output file name, using `%outfile:`
    - starting row (data are added to the XLS file in columns always starting
      from the same row), using `%startrow`
    - mass conversion factor, using `%mass:`
    - time conversion factor, using `%time:`
    - power conversion factor, using `%power:`

 - the second part lists each information that will be written in the XLS
   file, each line needs to be formatted as:
   `sheetX.xml:COL:INTEL:FAC_A1,FAC_A2,...:FAC_B1,FAC_B2,...:COM_1,COM_2,...:NUC_1,NUC_2...`,
   where :
     - X needs to be replaced with the sheet number,
     - CAL by a column name (A,B,... AZ ...),
     - INTEL by the information to recover, `time` for the timestep list, `power` for the power timeserie, `trans` for transaction, and
   `inv` for the inventory. 
   
   When requesting power or time,
   facilities/commodities/nuclides information are not use. When requesting
   inventory only the first FAC_A and NUC are read as a list a facilities and a
   list of nuclides, respectively. When requestion transaction, the FAC_A list
   corresponds to the senders, FAC_B to the receivers facilities, and COM to the
   commodities.
   The item separator in the list is the `,`.
   Some example of input file can be found in [input_example](input_example).

