using UnityEngine;

public class CameraMover : MonoBehaviour
{
    bool move = false;
    float speed = 0.01f;
    float offset = 0;
    Vector3 startPosition;
    Vector3 needPosition;
    Quaternion startRotation;
    Quaternion needRotaton;

    public void Move()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(-0.26f, 2.6f, 0.46f);
        needRotaton = Quaternion.AngleAxis(0, new Vector3(0, 1, 0));
    }

    public void Move1()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(-0.97f, 2.6f, 0.46f);
        needRotaton = Quaternion.AngleAxis(0, new Vector3(0, 1, 0));
    }

    public void Move2()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(1.61f, 2.31f, -0.85f);
        needRotaton = Quaternion.AngleAxis(0, new Vector3(0, 1, 0));
    }

    public void Move3()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(-0.96f, 2.49f, -0.533f);
        needRotaton = Quaternion.AngleAxis(70, new Vector3(1, 0, 0));
    }

    public void Move4()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(-1.00f, 2.5f, -0.49f);
        needRotaton = Quaternion.AngleAxis(30, new Vector3(1, 0, 0));
    }

    public void Move5()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(-1.00f, 2.5f, -0.49f);
        needRotaton = Quaternion.AngleAxis(30, new Vector3(1, 0, 0));
    }

    public void Move6()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(1.06f, 2.61f, 0.19f);
        needRotaton = Quaternion.AngleAxis(0, new Vector3(0, 1, 0));
    }

    public void Move7()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(-0.23f, 2.3f, 0.39f);
        needRotaton = Quaternion.AngleAxis(0, new Vector3(0, 1, 0));
    }

    public void Move0()
    {
        move = true;
        startPosition = transform.position;
        startRotation = transform.rotation;
        needPosition = new Vector3(-0.32f, 2.88f, -2.93f);
        needRotaton = Quaternion.AngleAxis(0, new Vector3(0, 1, 0));
    }

    void Update()
    {
        if (move)
        {
            offset += speed;
            transform.position = Vector3.Lerp(startPosition, needPosition, offset);
            transform.rotation = Quaternion.Slerp(startRotation, needRotaton, offset);

            if (offset >= 1)
            {
                move = false;
                offset = 0;
            }
        }
    }
}