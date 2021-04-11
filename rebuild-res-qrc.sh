#!/usr/bin/env bash


content="<!DOCTYPE RCC>
  <RCC version=\"1.0\">
    <qresource>"

for i in $(find ./res -name \* -type f -printf '%P\n');
do
    content="$content
      <file>$i</file>"

done

content="$content
  </qresource>
</RCC>"

echo "$content" > ./res/resources.qrc
