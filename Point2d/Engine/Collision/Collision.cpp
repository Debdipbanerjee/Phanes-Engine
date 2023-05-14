#include "Collision.h"

std::vector<Engine::Collision::CollisionComponent*> Engine::Collision::CollisionComponent::CollisionList = std::vector<CollisionComponent*>();

void Engine::Collision::CollisionComponent::AddCollisionComponent(Engine::Pointer::SmartPtr<GameObject> gameobject, nlohmann::json& collision)
{
	CollisionComponent* CC = new CollisionComponent(gameobject);

	nlohmann::json Collision = collision;

	if (Collision.contains("centre"))
	{
		CC->aabb->centre.posX = Collision["centre"][0];
		CC->aabb->centre.posY = Collision["centre"][1];
	}

	if (Collision.contains("extents"))
	{
		CC->aabb->extents.posX = Collision["extents"][0];
		CC->aabb->extents.posY = Collision["extents"][1];
	}

	CollisionList.push_back(CC);
}

Engine::Collision::CollisionComponent* Engine::Collision::CollisionComponent::GetCollisionComponent(Engine::Pointer::SmartPtr<GameObject> gameobject)
{	
	for (int i = 0; i < CollisionList.size(); i++)
	{
		Engine::Pointer::SmartPtr<GameObject> SmartGo = Engine::Pointer::SmartPtr<GameObject>(CollisionList[i]->WeakGo);
		if (SmartGo == gameobject)
		{
			return CollisionList[i];
		}
	}

	return nullptr;
}

void Engine::Collision::CollisionComponent::AddCollisionEvent(std::function<void(CollisionComponent* other)> Event)
{
	CollisionHandle = Event;
}

void Engine::Collision::CollisionComponent::Update(float dt)
{
	for (auto i : CollisionComponent::CollisionList)
	{
		Engine::Pointer::SmartPtr<GameObject> ObjA(i->WeakGo);
		for (auto j : CollisionComponent::CollisionList)
		{
			Engine::Pointer::SmartPtr<GameObject> ObjB(j->WeakGo);
			if (ObjB == ObjA)
			{
				// do nothing, due to no (!=) operator overload
			}
			else
			{
				if (i->IsCollidable && j->IsCollidable)
				{
					bool collision = checkCollision(dt, ObjB, ObjA, j, i);
					if (collision && i->CollisionHandle != nullptr)
					{
						i->CollisionHandle(j);
						if (j->CollisionHandle)
						{
							j->CollisionHandle(i);
						}
					}
				}
			}
		}
	}
}

bool Engine::Collision::CollisionComponent::checkCollision(float dt, Engine::Pointer::SmartPtr<GameObject> ObjA, Engine::Pointer::SmartPtr<GameObject> ObjB, CollisionComponent* ObjACol, CollisionComponent* ObjBCol)
{
	float tStart = 0.0f;
	float tEnd = dt;
	float tClose = 0;
	float tOpen = 0;

	// A Collision in B
	{
		Engine::Math::Matrix ObjAWorld = Engine::Math::Matrix::CreateTranslation(ObjA->Position.x, ObjA->Position.y, 1);
		Engine::Math::Matrix ObjBWorld = Engine::Math::Matrix::CreateTranslation(ObjB->Position.x, ObjB->Position.y, 1);
		Engine::Math::Matrix WorldObjB = ObjBWorld.GetNewInvertMatrix();
		Engine::Math::Matrix ObjAObjB = WorldObjB * ObjAWorld;

		Engine::Math::Vector4D ObjAPositionRelativeToObjB = ObjAObjB.VectorRightMulitply(Engine::Math::Vector4D(ObjA->Position.x, ObjA->Position.y, 1.0f, 1.0f));

		if (ObjACol && ObjBCol)
		{
			Engine::Math::Vector4D ObjACenterObjB = ObjAObjB.VectorRightMulitply(Engine::Math::Vector4D(ObjACol->aabb->centre.posX, ObjACol->aabb->centre.posY, 1.0f, 1.0f));
			Engine::Math::Vector4D ObjAExXObjB = ObjAObjB.VectorRightMulitply(Engine::Math::Vector4D(ObjACol->aabb->extents.posX, 0.0f, 0.0f, 0.0f));
			Engine::Math::Vector4D ObjAExYObjB = ObjAObjB.VectorRightMulitply(Engine::Math::Vector4D(0.0f, ObjACol->aabb->extents.posY, 0.0f, 0.0f));

			float ObjAProjectionObjB = fabs(ObjAExXObjB.x) + fabs(ObjAExYObjB.x);
			Engine::Math::Vector4D ObjACentreObjB = ObjAObjB.VectorRightMulitply(Engine::Math::Vector4D(ObjACol->aabb->centre.posX, ObjACol->aabb->centre.posY, 1.0f, 1.0f));

			float ObjALeftEdgeObjB = ObjACentreObjB.x - ObjAProjectionObjB;
			float ObjARightEdgeObjB = ObjACentreObjB.x + ObjAProjectionObjB;
			float ObjATopEdgeObjB = ObjACentreObjB.y + ObjAProjectionObjB;
			float ObjADownEdgeObjB = ObjACentreObjB.y - ObjAProjectionObjB;

			Engine::Math::Point2d ARelVelToB = Engine::Math::Point2d( ObjA->Velocity.x - ObjB->Velocity.x, ObjA->Velocity.y - ObjB->Velocity.y);
			Engine::Math::Vector4D ARelVelToBInB = WorldObjB.VectorRightMulitply(Engine::Math::Vector4D(ARelVelToB.posX, ARelVelToB.posY, 0, 0));

			float ObjBLeftEdge = ObjBCol->aabb->centre.posX - ObjBCol->aabb->extents.posX;
			float ObjBRightEdge = ObjBCol->aabb->centre.posX + ObjBCol->aabb->extents.posX;

			float tClAX = (ObjBLeftEdge - ObjARightEdgeObjB) / ARelVelToBInB.x;
			float tOpAX = (ObjBRightEdge - ObjALeftEdgeObjB) / ARelVelToBInB.x;

			tClose = tClAX;
			tOpen = tOpAX;

			if (tOpAX < tClAX)
			{
				std::swap(tOpAX, tClAX);
			}

			if (tClAX > tEnd)
			{
				return false;
			}

			if (tOpAX < 0)
			{
				return false;
			}

			if (ARelVelToBInB.x == 0 && ObjACentreObjB.x < ObjBLeftEdge && ObjACentreObjB.x > ObjBRightEdge)
			{
				return false;
			}

			float ObjBTopEdge = ObjBCol->aabb->centre.posY - ObjBCol->aabb->extents.posY;
			float ObjBDownEdge = ObjBCol->aabb->centre.posY + ObjBCol->aabb->extents.posY;

			float tClAY = (ObjBTopEdge - ObjADownEdgeObjB) / ARelVelToBInB.y;
			float tOpAY = (ObjBDownEdge - ObjATopEdgeObjB) / ARelVelToBInB.y;

			if (tOpAY < tClAY)
			{
				std::swap(tOpAY, tClAY);
			}

			if (tOpAY < tOpen)
			{
				tOpen = tOpAY;
			}

			if (tClAY > tClose)
			{
				tClose = tClAY;
			}

			if (tClAY > tEnd)
			{
				return false;
			}

			if (tOpAY < 0)
			{
				return false;
			}

			if (ARelVelToBInB.y == 0 && ObjACentreObjB.y < ObjBTopEdge && ObjACentreObjB.y > ObjBDownEdge)
			{
				return false;
			}
		}
	}

	// B Collision in A
	{
		Engine::Math::Matrix ObjAWorld = Engine::Math::Matrix::CreateTranslation(ObjA->Position.x, ObjA->Position.y, 1);
		Engine::Math::Matrix ObjBWorld = Engine::Math::Matrix::CreateTranslation(ObjB->Position.x, ObjB->Position.y, 1);
		Engine::Math::Matrix WorldObjA = ObjAWorld.GetNewInvertMatrix();
		Engine::Math::Matrix ObjBObjA = WorldObjA * ObjBWorld;

		Engine::Math::Vector4D ObjBPositionRelativeToObjA = ObjBObjA.VectorRightMulitply(Engine::Math::Vector4D(ObjB->Position.x, ObjB->Position.y, 1.0f, 1.0f));

		if (ObjACol && ObjBCol)
		{
			Engine::Math::Vector4D ObjBCenterObjA = ObjBObjA.VectorRightMulitply(Engine::Math::Vector4D(ObjBCol->aabb->centre.posX, ObjBCol->aabb->centre.posY, 1.0f, 1.0f));
			Engine::Math::Vector4D ObjBExXObjA = ObjBObjA.VectorRightMulitply(Engine::Math::Vector4D(ObjBCol->aabb->extents.posX, 0.0f, 0.0f, 0.0f));
			Engine::Math::Vector4D ObjBExYObjA = ObjBObjA.VectorRightMulitply(Engine::Math::Vector4D(0.0f, ObjBCol->aabb->extents.posY, 0.0f, 0.0f));

			float ObjBProjectionObjA = fabs(ObjBExXObjA.x) + fabs(ObjBExYObjA.x);
			Engine::Math::Vector4D ObjBCentreObjA = ObjBObjA.VectorRightMulitply(Engine::Math::Vector4D(ObjBCol->aabb->centre.posX, ObjBCol->aabb->centre.posY, 1.0f, 1.0f));

			float ObjBLeftEdgeObjA = ObjBCentreObjA.x - ObjBProjectionObjA;
			float ObjBRightEdgeObjA = ObjBCentreObjA.x + ObjBProjectionObjA;
			float ObjBTopEdgeObjA = ObjBCentreObjA.y + ObjBProjectionObjA;
			float ObjBDownEdgeObjA = ObjBCentreObjA.y - ObjBProjectionObjA;

			Engine::Math::Point2d BRelVelToA = Engine::Math::Point2d(ObjB->Velocity.x - ObjA->Velocity.x, ObjB->Velocity.y - ObjA->Velocity.y);
			Engine::Math::Vector4D BRelVelToAInA = WorldObjA.VectorRightMulitply(Engine::Math::Vector4D(BRelVelToA.posX, BRelVelToA.posY, 0, 0));

			float ObjALeftedge = ObjACol->aabb->centre.posX - ObjACol->aabb->extents.posX;
			float ObjARightedge = ObjACol->aabb->centre.posX + ObjACol->aabb->extents.posX;
			float ObjATopedge = ObjACol->aabb->centre.posY - ObjACol->aabb->extents.posY;
			float ObjADownedge = ObjACol->aabb->centre.posY + ObjACol->aabb->extents.posY;

			float tClAX = (ObjALeftedge - ObjBRightEdgeObjA) / BRelVelToAInA.x;
			float tOpAX = (ObjARightedge - ObjBLeftEdgeObjA) / BRelVelToAInA.x;


			if (tOpAX < tClAX)
			{
				std::swap(tOpAX, tClAX);
			}

			if (tClAX > tEnd)
			{
				return false;
			}

			if (tOpAX < 0)
			{
				return false;
			}

			if (BRelVelToAInA.x == 0 && ObjBCentreObjA.x < ObjALeftedge && ObjBCentreObjA.x > ObjARightedge)
			{
				return false;
			}

			if (tOpAX < tOpen)
			{
				tOpen = tOpAX;
			}

			if (tClAX > tClose)
			{
				tClose = tClAX;
			}

			float tClAY = (ObjATopedge - ObjBDownEdgeObjA) / BRelVelToAInA.y;
			float tOpAY = (ObjADownedge - ObjBTopEdgeObjA) / BRelVelToAInA.y;

			if (tOpAY < tClAY)
			{
				std::swap(tOpAY, tClAY);
			}

			if (tOpAY < tOpen)
			{
				tOpen = tOpAY;
			}

			if (tClAY > tClose)
			{
				tClose = tClAY;
			}

			if (tClAY > tEnd)
			{
				return false;
			}

			if (tOpAY < 0)
			{
				return false;
			}

			if (BRelVelToAInA.y == 0 && ObjBCentreObjA.y < ObjATopedge && ObjBCentreObjA.y > ObjADownedge)
			{
				return false;
			}
		}
	}

	bool status = (tClose < tOpen);

	return status;
}

