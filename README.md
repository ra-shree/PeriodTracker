# Period Tracker Software

## ABSTRACT
Period tracker is a software that takes information about past menstrual cycle dates and uses it predict the length of the next menstrual cycle. There is no application that can accurately track and predict a woman’s menstrual cycle for Windows systems.  Period Tracker allows women to stay on top of their reproductive health as well as catch any underlying health symptoms indicated by irregular menstrual cycles early. We expect our application to allow users to prepare for their next cycle as well as make informed decisions about their reproductive health

---

## LIST OF SYMBOLS
*	CLD - Cycle Length Difference
*   GUI - Graphical User Interface
*   GLFW - Graphics Library Framework
*	IDE	- Integrated Development Environment
*	LMCL - Latest Menstrual Cycle Length
*	PMCL - Last Predicted Menstrual Cycle Length
*	SD - Standard Deviation 

---

## INTRODUCTION
Motivated by longstanding problems such as difficulty in tracking irregular periods and to provide guidance to young adults to keep track of their mensuration and sexual health, the period tracker is a software that can predict the dates of the next menstrual cycle using previous data. It also provides data related to ovulation period, fertility window and visual representation of the amount of blood flow that user experience in one cycle. The Period Tracker stores information such as the dates of past monthly cycles, whether the last predicted date was correct, the range of error in the prediction and uses this data to predict the dates in the next monthly cycle before informing the user. It can also calculate the length of the menstrual period as well as the length of the ovulation period.
The purpose of this project is to reach woman who are unable to keep daily track of their periods, who can’t afford to visit hospital just to know about their fertility window when they want to conceive.

---

## PROBLEM STATEMENT
Acknowledging the problem of 54.19% population of our country and enabling them to relay on period tracker for better prediction of irregular period and prepare them for their hectic mensuration days ahead. The lack of any popular software for Desktop or Windows Operating System related to tracking and predicting periods made us interested in this topic.

---

## PROJECT OBJECTIVE 
* To build a period tracking software that can predict the dates of the next menstrual cycle using previous data.
* To predict irregular periods and also determine fertility window in a menstrual cycle.

---

## LITERATURE REVIEW
There has been considerable amount of research in this field. Using big data, the results have shown that 87% of women had menstrual cycle lengths from 23 to 35 days. For half the women studied, their menstrual cycle varied from the median 28 days by 5 days or more with the ovulation cycle falling anywhere from day 8 to day 18 for a 28-day cycle and day 15 being the most common ovulation day [2]. Furthermore, with the help of supervised neural networks, a woman’s next menstrual cycle can be predicted with the length of the last 10 cycles [3].
Among commercially available applications, almost all of them are exclusive to either Android or IOS operating systems. Most of them track menstrual cycles, predict cycle length, preparation for conception and menopause [4].

---

## METHODOLOGY
The programming language used to build the tracker is C++. We will mostly be using the standard library with the only exception being Dear ImGUI which will be used to build the User Interface. We will be making use of the ctime and chrono library to retrieve system date and manipulate date objects. We will also use the vector library for the dynamic sized arrays and the fstream library to read and write files.

We will be using a statistics-based model to predict the next menstruation date. The data we will be keeping track of are – cycle length difference (CLD) which is the difference in lengths between successive menstrual cycles. Assuming D1, D2 and D3 are consecutive menstrual cycle lengths, CLD = D3 – D2 = D2 – D1 = Dn – Dn-1. A large variation in CLD is key information in determining the irregularity of an individual’s menstrual cycle. Another key point is the maximum and minimum lengths of the menstrual cycle being 35 days and 23 days respectively. Any menstrual length longer or shorter than these days respectively can be tagged as irregular menstrual cycle and the user needs to be informed. An irregular menstrual cycle either follows an upwards or a downwards trend, this can be discovered using the variation in CLD and the next date is predicted accordingly. 

We use the five-number summary to describe the nature of the distribution. The smallest and the largest numbers are 23 and 35 respectively. The initial menstrual cycle length is set at 28 days since it is the median cycle length of most women.

For the next menstrual cycle, the length of the previous recorded cycle is used. Once we have 2 recorded cycle lengths, we calculate the CLD and for a CLD of ±5, we use the median of the two. For less CLD, we use the shorter of the recorded lengths and for high CLD, we use the longer recorded length. After recording more than 3 date we calculate the quartiles of the data. We also calculate the difference between the latest menstrual cycle length (LMCL) and the last predicted menstrual cycle length (PMCL) and the standard deviation of their mean (SD) to correct the next prediction.

### **ALGORITHM FOR PREDICTING NEXT MENSTRUAL CYCLE LENGTH**
**Input**: Previous Menstrual Cycle, CLD, Predicted Menstrual Cycle Lengths, Latest Cycle Length, SD, LMCL, PMCL
**Output**: Next Predicted Length, Ovulation [start, end]
**Process**:

STEP 1: Check for saved data.

STEP 2: If (no saved data)
a.	Next Predicted Length = 28
b.	GOTO STEP 7

STEP 3: Else 
a.	Calculate quartiles Q1, Q2, Q3 of Previous Menstrual Cycle 
b.	CLD = Latest Cycle Length – Previous Menstrual Cycle[n-1]
c.	If (CLD > -5 and CLD < 5)
Next Predicted Length = Q2
d.	Else If (CLD > 5)
Next Predicted Length = Q3 
e.	Else If (CLD < -5)
Next Predicted Length = Q2

STEP 4: Calculate Standard Deviation (SD) between PMCL and LMCL

STEP 5: Next Predicted Length += SD 

STEP 6: Ovulation [start, end] = [Next Predicted Length + 14 + SD, Next Predicted Length + 14 – SD]

STEP 7: STOP

![System Flowchart](/assets/Flowchart.png)
---

## EXPECTED OUTCOME

Period Tracker provides an easy way for women to stay informed on of their menstrual cycle and make informed decisions related to their reproductive health. The information provided by the tracker can be used to plan or avoid unwanted pregnancy. It also tracks any irregularity in the menstrual cycle which might be critical in finding any associated health problems early.

---

## REFERENCES
[1] Trading Economics, "Trading Economics," 2020. [Online]. Available: https://tradingeconomics.com/nepal/population-female-percent-of-total-wb-data.html. [Accessed February 2022].

[2] B. G. S. J. I Soumpasis, "Real-life insights on menstrual cycles and ovulation using big data," Human Reproduction Open, vol. 2020, no. 2, p. hoaa011, 2020. 

[3] I. U. A. S. V. J. V. C. H. W. N. E. Kathy Li, "A predictive model for next cycle start date that accounts for adherence in menstrual self-tracking," Journal of the American Medical Informatics Association, vol. 29, no. 1, pp. 3 - 11, 2022. 

[4] Flo Health, Inc, "Flo Ovulation & Period Tracker," 2022. [Online]. Available: https://play.google.com/store/apps/details?id=org.iggymedia.periodtracker&hl=en&gl=US. [Accessed February 2022].

[5] Flo Health, Inc, "Flo," 2022. [Online]. Available: https://flo.health/. [Accessed February 2022].

[6] S. L. A. Mitchell D.Creinina, "How regular is regular? An analysis of menstrual cycle regularity," Department of Obstetrics, Gynecology and Reproductive Sciences, University of Pittsburgh School of Medicine and Magee Womens Research Institute, 300 Halket Street, Pittsburgh, PA 15213-3180, USA, 2004.

[7] M. S. K. R. Richard J. Fehring, "Variability in the Phases of the Menstrual Cycle," JOGNN, vol. 35, no. 3, pp. 376-384, 2006. 


---
## Resources
 * [Dear ImGUI](https://github.com/ocornut/imgui) - The GUI framework in use with OpenGL as the renderer and GLFW to handle input
 * [The Cherno](https://www.youtube.com/playlist?list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb) - C++ resource
 * [cppreference](https://en.cppreference.com/w/) - Great C++ Library Reference Website


## Tools Used
 * Visual Studio 2022
 * Github
