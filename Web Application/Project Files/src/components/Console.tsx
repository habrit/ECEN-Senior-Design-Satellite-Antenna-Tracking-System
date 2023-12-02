import * as React from "react";
import { styled } from "@mui/material/styles";
import Typography from "@mui/material/Typography";
import SatelliteTable from "./SatelliteTable";
import Dashboard from "./Dashboard";
import { Icon, Modal } from "@mui/material";
import Tabs from "@mui/material/Tabs";
import Tab from "@mui/material/Tab";
import Box from "@mui/material/Box";

interface StyledTabsProps {
  children?: React.ReactNode;
  value: number;
  onChange: (event: React.SyntheticEvent, newValue: number) => void;
}

const StyledTabs = styled((props: StyledTabsProps) => (
  <Tabs
    {...props}
    TabIndicatorProps={{ children: <span className="MuiTabs-indicatorSpan" /> }}
  />
))({
  "&.MuiTabs-root": {
    minHeight: "24px",
  },
  "& .MuiTabs-indicator": {
    display: "flex",
    justifyContent: "center",
    backgroundColor: "transparent",
  },
  "& .MuiTabs-indicatorSpan": {
    maxWidth: 40,
    //width: "100%",
    //backgroundColor: "blue",
  },
});

interface StyledTabProps {
  label: string; // Add the label prop
}

const StyledTab = styled((props: StyledTabProps) => (
  <Tab disableRipple {...props} />
))(({ theme }) => ({
  textTransform: "none",
  fontFamily: "Roboto Mono, monospace",
  fontWeight: theme.typography.fontWeightRegular,
  fontSize: theme.typography.pxToRem(15),
  marginRight: theme.spacing(1),
  color: "#8C92A4",
  "&.Mui-selected": {
    color: "white",
    zIndex: 1,
    "&.MuiTab-root":{
      backgroundImage: `url("data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='129' height='26' fill='none'><path fill='%23181C20' d='M21 0C10.648 0 15.109 26 0 26h129c-18.865 0-12.944-26-22.5-26H21Z'/></svg>")`,
    }
 },
  "&.Mui-focusVisible": {
    //backgroundColor: "rgba(100, 95, 228, 0.32)",
  },
  "&:hover":{
    color:"white"
  },
  "&.MuiTab-root": {
    
    height: "26px",
    minHeight: "26px",
    width: "130px",
    backgroundImage: `url("data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='129' height='26' fill='none'><path fill='%23121417' d='M21 0C10.648 0 15.109 26 0 26h129c-18.865 0-12.944-26-22.5-26H21Z'/></svg>")`,
    backgroundSize: "cover",
    paddingTop: "0px",
    paddingBottom: "0px",
  },
}));

interface TabPanelProps {
  children?: React.ReactNode;
  index: number;
  value: number;
}

function CustomTabPanel(props: TabPanelProps) {
  const { children, value, index, ...other } = props;

  return (
    <div
      role="tabpanel"
      hidden={value !== index}
      id={`simple-tabpanel-${index}`}
      aria-labelledby={`simple-tab-${index}`}
      {...other}
    >
      {value === index && (
        <Box
          sx={{
            p: 3,
            padding: "12px",
            backgroundColor: "#181C20",
            //borderRadius: "8px",
          }}
        >
          {children}
 
        </Box>
      )}
    </div>
  );
}

function a11yProps(index: number) {
  return {
    id: `simple-tab-${index}`,
    "aria-controls": `simple-tabpanel-${index}`,
  };
}

export default function CustomizedTabs() {
  const [value, setValue] = React.useState(0);

  const handleChange = (event: React.SyntheticEvent, newValue: number) => {
    setValue(newValue);
  };

  return (
    <>
      <Box sx={{ width: "100%" }}>
        <Box>
          <StyledTabs value={value} onChange={handleChange}>
            <StyledTab
           
              label="Dashboard"
              {...a11yProps(0)}
              sx={{ marginLeft: "4px",marginRight: "-10px" }}
            />
            <StyledTab
              label="Satellites"
              {...a11yProps(1)}
              sx={{ marginLeft: "-20px" }}
            />
          </StyledTabs>
          <CustomTabPanel value={value} index={1}>
            <SatelliteTable />
          </CustomTabPanel>
          <CustomTabPanel value={value} index={0}>
            <Dashboard setValue={setValue} />
          </CustomTabPanel>
        </Box>
      </Box>
    </>
  );
}
