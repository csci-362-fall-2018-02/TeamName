#!/bin/bash

FILE = "./output.html"

mapfile -t my_array < <( ls )
declare -p my_array
{
  echo "<!DOCTYPE html>"
  echo "<html>"
  echo "<head>"
  echo "<style>"
  echo "table, th, td {"
  echo "  border: 1px solid black;"
  echo "}"
  echo "</style>"
  echo "</head>"
  echo "<body>"
  echo "<h1>Files in Directory</h1>"
  echo "<table style=\"width:100%\">"
} > FILE

for i in "${my_array[@]}"
do
  {
    echo "  <tr>"
    echo "    <td>${i}</td>"
    echo "  </tr>"
  } >> FILE
done

{
  echo "</table>"
  echo "</body>"
  echo "</html>"
} >> FILE
