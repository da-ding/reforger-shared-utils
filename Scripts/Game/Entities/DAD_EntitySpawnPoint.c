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
	protected ref SCR_UIInfo m_EntityInfo;

	/* [RplProp(onRplName: "OnSetEntityID")] */
	protected RplId m_EntityID;

	protected IEntity m_TargetEntity;

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

	/*!
	Assign entity ID to this respawn point.
	The Spawn will then activate and follow the position of the entity
	\param entityID Target entity ID
	*/
	void SetEntityID(RplId entityID)
	{
		if (entityID == m_EntityID || !Replication.IsServer())
			return;

		//--- Set and broadcast new entity ID
		m_EntityID = entityID;
		OnSetEntityID();
		Replication.BumpMe();

		RplComponent rplC = RplComponent.Cast(Replication.FindItem(m_EntityID));
		IEntity entity = rplC.GetEntity();
		//IEntity entity = GetGame().GetWorld().FindEntityByName(m_EntityID);
		// TODO: Set listener to auto-update when entity is deleted from world
		if (entity)
			OnEntitySpawn(m_EntityID, entity);
		else
			OnEntityDeleted(m_EntityID, null);
	}

	void SetEntity(IEntity entity)
	{
		RplComponent rplC = RplComponent.Cast(entity.FindComponent(RplComponent));
		RplId id = rplC.Id();
		SetEntityID(id);
	}

	/*!
	Get ID of the player this spawn point is assigned to.
	\return Target entity ID
	*/
	RplId GetEntityID()
	{
		return m_EntityID;
	}

	protected void OnSetEntityID()
	{
		LocalizedString name = m_EntityID.ToString();
		//--- Link player info
		if (!m_EntityInfo)
			m_EntityInfo = SCR_UIInfo.CreateInfo(name);
		LinkInfo(m_EntityInfo);
	}

	protected void OnEntitySpawn(RplId entityID, IEntity entity)
	{
		if (entityID != m_EntityID)
			return;

		m_TargetEntity = entity;
		ActivateSpawnPoint();
	}

	protected void OnEntityDeleted(RplId entityID, IEntity entity)
	{
		if (entityID != m_EntityID)
			return;

		DeactivateSpawnPoint();
		m_TargetEntity = null;
	}

	protected void ActivateSpawnPoint()
	{
		m_bActivated = true;
		//--- Periodically refresh spawn's position
		//--- Clients cannot access another player's entity directly, because it may not be streamed for them
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
		UpdateSpawnPosBroadcast(pos);
		Rpc(UpdateSpawnPosBroadcast, pos);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void UpdateSpawnPosBroadcast(vector pos)
	{
		SetOrigin(pos);
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
