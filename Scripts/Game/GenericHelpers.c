class GenericHelpers
{	
	static void SnapToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);
		
		SCR_TerrainHelper.SnapToTerrain(transform);
		entity.SetTransform(transform);
	}
	
	static void SnapAndOrientToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);
		
		SCR_TerrainHelper.SnapAndOrientToTerrain(transform);
		entity.SetTransform(transform);
	}
	
}
