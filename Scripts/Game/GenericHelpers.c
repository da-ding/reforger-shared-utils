class GenericHelpers
{	
	static void SnapToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);
		
		SCR_TerrainHelper.SnapToTerrain(transform);
		entity.SetTransform(transform);
	}

}