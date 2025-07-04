using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class InstrumentController : MonoBehaviour, IPointerClickHandler
{
    public Transform voltmeter;
    public Transform ammeter;
    public Transform water;
    public TextMesh lowerTempText;
    public TextMesh upperTempText;

    public Animator fanAnimator; 
    public Animator switchAnimator; 
    public GameObject fanObject; 
    public GameObject switchObject;  

    private bool isFanAnimating = false;  
    private bool areInstrumentsActive = false; 
    private bool voltmeterTarget = false;
    private bool ammeterTarget = false;

    // Вольтметр
    private float voltmeterAngle = -90f;
    private float voltmeterEndAngle = -140f;
    private float voltmeterFluctuationRange = 3f;
    private float voltmeterSpeed = 0.2f;

    // Амперметр
    private float ammeterAngle = -90f;
    private float ammeterEndAngle = -130f;
    private float ammeterFluctuationRange = 2f;
    private float ammeterSpeed = 0.15f;

    // Температура
    private float lowerTempMin = 0f;
    private float lowerTempMax = 200f;
    private float upperTempMin = 0f;
    private float upperTempMax = 125f;
    private float tempSpeed = 0.1f;
    private float lowerTempSpeed = 0.4f;
    private float upperTempSpeed = 0.3f;

    // Вода
    private float maxScaleZ = 1f;
    private float minScaleZ = 0f;
    private float waterSpeed = 0.1f;

    void Start()
    {
        fanAnimator = fanObject.GetComponent<Animator>();
        switchAnimator = switchObject.GetComponent<Animator>();
        SetNeedleRotation(voltmeter, voltmeterAngle);
        SetNeedleRotation(ammeter, ammeterAngle);
        lowerTempText.text = FormatTemperature(lowerTempMin);
        upperTempText.text = FormatTemperature(0f);
        water.localScale = new Vector3(water.localScale.x, water.localScale.y, minScaleZ);
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        if (eventData.pointerCurrentRaycast.gameObject == fanObject)
        {
            isFanAnimating = !isFanAnimating;

            if (isFanAnimating)
            {
                fanAnimator.SetTrigger("hittenOn");  
                Debug.Log("Fan Animation Started");
            }
            else
            {
                fanAnimator.SetTrigger("hittenOff");  
                Debug.Log("Fan Animation Stopped");
            }
        }
        else if (eventData.pointerCurrentRaycast.gameObject == switchObject)
        {
            areInstrumentsActive = !areInstrumentsActive;

            if (areInstrumentsActive)
            {
                switchAnimator.SetTrigger("hittenOn");  
                Debug.Log("Instruments Activated");
            }
            else
            {
                switchAnimator.SetTrigger("hittenOff");  
                Debug.Log("Instruments Deactivated");
            }
        }
    }

    void Update()
    {
        if (areInstrumentsActive)
        {
            UpdateInstrumentsActivation();
        }
        else
        {
            UpdateInstrumentsDeactivation();
        }
    }

    private void UpdateInstrumentsActivation()
    {
        if (!voltmeterTarget)
        {
            voltmeterAngle = Mathf.Lerp(voltmeterAngle, voltmeterEndAngle, Time.deltaTime * voltmeterSpeed);
            if (Mathf.Abs(voltmeterAngle - voltmeterEndAngle) < 0.1f)
            {
                voltmeterAngle = voltmeterEndAngle;
                voltmeterTarget = true;
            }
        }
        else
        {
            voltmeterAngle = voltmeterEndAngle + Mathf.Sin(Time.time) * voltmeterFluctuationRange;
        }

        if (!ammeterTarget)
        {
            ammeterAngle = Mathf.Lerp(ammeterAngle, ammeterEndAngle, Time.deltaTime * ammeterSpeed);
            if (Mathf.Abs(ammeterAngle - ammeterEndAngle) < 0.1f)
            {
                ammeterAngle = ammeterEndAngle;
                ammeterTarget = true;
            }
        }
        else
        {
            ammeterAngle = ammeterEndAngle + Mathf.Sin(Time.time * 1.5f) * ammeterFluctuationRange;
        }

        SetNeedleRotation(voltmeter, voltmeterAngle);
        SetNeedleRotation(ammeter, ammeterAngle);

        /*        float collision = Mathf.Sin(Time.time * collisionSpeed) * 5f;
                float targetUpperTemp = upperTempMax + collision;*/

        if (lowerTempMin < lowerTempMax)
        {
            lowerTempMin = Mathf.Lerp(lowerTempMin, lowerTempMax, Time.deltaTime * lowerTempSpeed);
            lowerTempText.text = FormatTemperature(lowerTempMin);
        }

        if (upperTempMin < upperTempMax)
        {
            upperTempMin = Mathf.Lerp(upperTempMin, upperTempMax, Time.deltaTime * upperTempSpeed);
            upperTempText.text = FormatTemperature(upperTempMin);
        }

        float currentScaleZ = water.localScale.z;
        if (currentScaleZ < maxScaleZ)
        {
            float newScaleZ = Mathf.Lerp(currentScaleZ, maxScaleZ, Time.deltaTime * waterSpeed);
            water.localScale = new Vector3(water.localScale.x, water.localScale.y, Mathf.Clamp(newScaleZ, minScaleZ, maxScaleZ));
        }
        else
        {
            water.localScale = new Vector3(water.localScale.x, water.localScale.y, maxScaleZ);
        }
    }

    private void UpdateInstrumentsDeactivation()
    {
        voltmeterAngle = Mathf.Lerp(voltmeterAngle, -90f, Time.deltaTime * voltmeterSpeed);
        SetNeedleRotation(voltmeter, voltmeterAngle);

        ammeterAngle = Mathf.Lerp(ammeterAngle, -90f, Time.deltaTime * ammeterSpeed);
        SetNeedleRotation(ammeter, ammeterAngle);

        lowerTempMin = Mathf.Lerp(lowerTempMin, 0f, Time.deltaTime * tempSpeed);
        upperTempMin = Mathf.Lerp(upperTempMin, 0f, Time.deltaTime * tempSpeed);
        lowerTempText.text = FormatTemperature(lowerTempMin);
        upperTempText.text = FormatTemperature(upperTempMin);

        float currentScaleZ = water.localScale.z;
        float newScaleZ = Mathf.Lerp(currentScaleZ, minScaleZ, Time.deltaTime * waterSpeed);
        water.localScale = new Vector3(water.localScale.x, water.localScale.y, newScaleZ);
    }

    private void SetNeedleRotation(Transform needle, float angle)
    {
        needle.localRotation = Quaternion.Euler(180, 0, angle);
    }

    private string FormatTemperature(float value)
    {
        return Mathf.RoundToInt(value).ToString("D4");
    }
}