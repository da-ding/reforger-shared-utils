class GenericHelpers
{	
	static void SnapToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);
		
		SCR_TerrainHelper.SnapToTerrain(transform);
		transform[3] = transform[3] + "0 10 0";
		entity.SetTransform(transform);
	}
	
	static void SnapAndOrientToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);
		
		SCR_TerrainHelper.SnapAndOrientToTerrain(transform);
		transform[3] = transform[3] + "0 10 0";
		entity.SetTransform(transform);
	}
	
}
