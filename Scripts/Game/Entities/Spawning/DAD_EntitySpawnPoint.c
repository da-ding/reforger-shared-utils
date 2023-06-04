[EntityEditorProps(category: "GameScripted/GameMode", description: "")]
class DAD_EntitySpawnPointClass: SCR_SpawnPointClass
{
};

/*!
 * A generic version of PlayerSpawnPointClass for any entity kind
 */
class DAD_EntitySpawnPoint: SCR_SpawnPoint
{
	[Attribute("1", desc: "How often will the spawn's position be updated (in seconds).", category: "Entity Spawn Point")]
	protected float m_fUpdateInterval;

	[Attribute(desc: "Spawn point visualization. Original 'Info' attribute will be ignored.", category: "Entity Spawn Point")]
	protected ref SCR_UIInfo m_TargetEntityInfo;

	protected IEntity m_TargetEntity = null;

	protected bool m_bActivated = true;

	IEntity GetTargetEntity()
	{
		return m_TargetEntity;
	}

	override string GetFactionKey()
	{
		if (!m_bActivated) return string.Empty;
		return super.GetFactionKey();
	}

	void SetEntity(IEntity entity)
	{
		if (entity == m_TargetEntity || !Replication.IsServer())
			return;

		m_TargetEntity = entity;
		OnEntitySpawn(entity);
	}

	protected void OnSetentity()
	{
		LocalizedString name = m_TargetEntity.ToString();
		if (!m_TargetEntityInfo)
			m_TargetEntityInfo = SCR_UIInfo.CreateInfo(name);
		LinkInfo(m_TargetEntityInfo);
	}

	// When the entity attached to the spawn point is spawned
	protected void OnEntitySpawn(IEntity entity)
	{
		if (entity != m_TargetEntity)
			return;

		m_TargetEntity = entity;
		ActivateSpawnPoint();
	}

	// When the entity attached to the spawn point is deleted
	protected void OnEntityDeleted(IEntity entity)
	{
		if (entity != m_TargetEntity)
			return;

		DeactivateSpawnPoint();
		m_TargetEntity = null;
	}

	protected void ActivateSpawnPoint()
	{
		m_bActivated = true;
		//--- Periodically refresh spawn's position
		ClearFlags(EntityFlags.STATIC, false);
		GetGame().GetCallqueue().CallLater(UpdateSpawnPos, m_fUpdateInterval * 1000, true);
	}

	protected void DeactivateSpawnPoint()
	{
		m_bActivated = false;
		//--- Stop periodic refresh
		SetFlags(EntityFlags.STATIC, false);
		GetGame().GetCallqueue().Remove(UpdateSpawnPos);
	}

	protected void UpdateSpawnPos()
	{
		if (!m_TargetEntity)
			return;

		vector pos = m_TargetEntity.GetOrigin();
		if (GetOrigin() == pos) return;

		// I don't know why this has multiple update spawn requests, but I suspect it's redundant in order to improve reliability.
		UpdateSpawnPosBroadcast(pos);
		Rpc(UpdateSpawnPosBroadcast, pos);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void UpdateSpawnPosBroadcast(vector pos)
	{
		SetOrigin(pos);
		Replication.BumpMe();
	}

	override void GetPositionAndRotation(out vector pos, out vector rot)
	{
		super.GetPositionAndRotation(pos, rot);

		if (m_TargetEntity)
		{
			IEntity vehicle  = GenericHelpers.GetVehicle(m_TargetEntity);
			if (vehicle)
			{
				rot = vehicle.GetAngles();
				return;
			}
			rot = m_TargetEntity.GetAngles();
		}
	}

	override void EOnPlayerSpawn(IEntity entity)
	{
		//--- If spawn point target is sitting in a vehicle, move spawned player inside as well
		IEntity vehicle = GenericHelpers.GetVehicle(m_TargetEntity);
		if (vehicle)
		{
			SCR_CompartmentAccessComponent compartmentAccessPlayer = SCR_CompartmentAccessComponent.Cast(entity.FindComponent(SCR_CompartmentAccessComponent));
			GetGame().GetCallqueue().CallLater(compartmentAccessPlayer.MoveInVehicleAny, 1, false, vehicle); //--- Wait for character ownership to be transferred to client
		}
	}
	void ~DAD_EntitySpawnPoint()
	{
		GetGame().GetCallqueue().Remove(UpdateSpawnPos);
	}
};
