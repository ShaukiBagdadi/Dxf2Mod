Make sure, before writing the DXF file, that the UCS is set to World!

DraftSight in 2011 when saving DXF was writing many digits for coords
and provided no way to set the precision level.
Because of that many nodes have to be merged with NMERGE and many 
bounding nodes are disconnected.
Therefore save DXF R12 with AutoDesk software setting the precision level!

Also when inserting FE-NODE with DraftSight - must be looking at the plane 
XY, otherwise the x, y and z values are scrambeled/replaced when writing the DXF

Check the model with SHOW_MERGE -- nothing should be merged with NMERGE to be
sure that the indexes of the generated GEO file mean the same as in the OUT file.

Check in GeoStar for merging elements.
