#include "Actor.h"
#include <array>


void frustumCullActors(Player& player, std::vector<Actor>& worldActors)
{
    std::array<std::array<float, 4>, 6> planes = player.getCamera().planes;

    for (Actor& actor : worldActors)
    {
        std::vector<glm::vec3> corners = actor.getBoundingBoxCorners();
        bool isCulled = true;

        for (int i = 0; i < 6; ++i)
        {
            bool insidePlane = false;

            for (const auto& corner : corners)
            {
                float distance = planes[i][0] * corner.x +
                                 planes[i][1] * corner.y +
                                 planes[i][2] * corner.z +
                                 planes[i][3];

                if (distance >= 0)
                {
                    insidePlane = true;
                    break;
                }
            }

            if (!insidePlane)
            {
                isCulled = true;
                break;
            }
            else
            {
                isCulled = false;
            }
        }

        actor.setCulled(isCulled);
    }
}
